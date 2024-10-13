#include "human_matting.h"






 static cv::Mat NNormalize(const cv::Mat& array, const std::vector<float>& mean, const std::vector<float>& std, int dtype = CV_32F) {
    cv::Mat im;
    array.convertTo(im, dtype);
    im = im / 255.0;
    cv::subtract(im, cv::Scalar(mean[0], mean[1], mean[2]), im);
    cv::divide(im, cv::Scalar(std[0], std[1], std[2]), im);
    return im;
}

static cv::Mat seg_preprocess( cv::Mat matBgrImg, int size_w, int size_h) {

    cv::Mat resize_img;
    cv::resize(matBgrImg, resize_img, cv::Size(size_w, size_h), 0, 0, cv::INTER_AREA);
    std::vector<float> mean = { 0.5f, 0.5f, 0.5f };
    std::vector<float> std = { 0.5f, 0.5f, 0.5f };
    cv::Mat normalized_image = NNormalize(resize_img, mean, std);
    return normalized_image;
}



static cv::Mat seg_postprocess(cv::Mat img_mat, cv::Mat input_image) {

    img_mat = (img_mat * 255);
    cv::Mat matte;
    cv::Mat resize_matte;
    img_mat.convertTo(matte, CV_8U);
    cv::resize(matte, resize_matte, input_image.size(),0,0, cv::INTER_AREA);
    std::vector<cv::Mat> bgr;
    cv::split(input_image, bgr); // bgr[0] 是蓝色通道，bgr[1] 是绿色通道，bgr[2] 是红色通道
 
    // 合并通道和掩码
    std::vector<cv::Mat> channels = { bgr[0], bgr[1], bgr[2], resize_matte };
  
    cv::merge(channels, resize_matte);
    return  resize_matte;



}




static cv::Mat hollow_out_fix(const cv::Mat& src) {
    cv::Mat src_img = src;
    if (src.channels() != 4 || src.dims != 2) {
        std::cerr << "Error: Image mismatch." << std::endl;
        return cv::Mat();
     }
    std::vector<cv::Mat> bgra(4);
    cv::Mat bgr_img;

    cv::split(src_img, bgra);
    cv::Mat b_img = bgra[0];
    cv::Mat g_img = bgra[1];
    cv::Mat r_img = bgra[2];
    cv::Mat a_img = bgra[3];
    std::vector<cv::Mat> bgr_channels;
    bgr_channels.push_back(b_img);
    bgr_channels.push_back(g_img);
    bgr_channels.push_back(r_img);

    cv::merge(bgr_channels, bgr_img);
    a_img=add_padding(a_img, 10, cv::BORDER_CONSTANT, cv::Scalar(0));
    cv::Mat a_threshold;
    cv::threshold(a_img, a_threshold, 127, 255, cv::THRESH_TOZERO);
    cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(5, 5));

    // 进行腐蚀操作
    cv::Mat eroded_image;
    cv::erode(a_threshold, eroded_image, kernel, cv::Point(-1, -1), 3);
    std::vector<std::vector<cv::Point>> contours;

    cv::findContours(eroded_image, contours, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_NONE);
    if (contours.size() == 0)  return src_img;
    std::sort(contours.begin(), contours.end(), compareContours);
    cv::Mat contourImg;
    contourImg= cv::Mat::zeros(a_img.size(), a_img.type());
    cv::drawContours(contourImg, contours, 0, cv::Scalar(255), 2);

    cv::Mat mask = cv::Mat::zeros(a_img.size() + cv::Size(2, 2), a_img.type());
    cv::Scalar newVal(255);
    cv::Point seedPoint(0, 0);
    cv::floodFill(contourImg, mask, seedPoint, newVal);
    cv::Mat maxVal = cv::Mat::ones(contourImg.size(), contourImg.type()) * 255;
    cv::Mat inverted_contour;
    cv::subtract(maxVal, contourImg, inverted_contour);
    cv::Mat updated_a;
    cv::add(a_img, inverted_contour, updated_a);
    cv::Mat slicedA = updated_a(cv::Range(10, updated_a.rows - 10), cv::Range(10, updated_a.cols - 10));

    // 合并 BGR 通道和更新后的 Alpha 通道
    std::vector<cv::Mat> channels;
    channels.push_back(bgr_img);
    channels.push_back(slicedA);

    cv::Mat result;
    cv::merge(channels, result);


    return result;

}




cv::Mat  human_matting(const char* & mnn_path, cv::Mat input_BgrImg,int num_thread) {

 
    cv::Mat matBgrImg =image3bgr(input_BgrImg);//判断图像通道
    if (matBgrImg.empty()) {
     std::cerr << "Error: Image cannot be loaded." << std::endl;
     return cv::Mat();
 }
    std::shared_ptr<Interpreter> net;
    net.reset(Interpreter::createFromFile(mnn_path));
    if (net == nullptr) {
        MNN_ERROR("Invalid Model\n");
        return cv::Mat();
    }
    ScheduleConfig config;
    config.numThread = num_thread;
    auto session = net->createSession(config);
    auto input = net->getSessionInput(session, nullptr);
    int size_w = SIZE_W;  // 初始化宽度
    int size_h = SIZE_H;  // 初试化高度
    auto shape = input->shape();
    if (shape[0] != 1) {
        shape[0] = 1;
    }
    if (shape[2] == -1) {
       shape[2] = size_h;
    }
    if (shape[3] == -1) {
        shape[3] = size_w;
    }
    net->resizeTensor(input, shape);
    net->resizeSession(session);
    
    cv::Mat pre_img = seg_preprocess(matBgrImg,size_w, size_h);
    std::vector<std::vector<cv::Mat>> nChannels;
    std::vector<cv::Mat> rgbChannels(3);
    cv::split(pre_img, rgbChannels);
    nChannels.push_back(rgbChannels); //  NHWC  转NCHW
    {
        void* pvData = malloc(1 * 3 * size_w * size_w * sizeof(float));
        if (pvData == nullptr) {
            printf("error");
        }
        int nPlaneSize = size_w * size_w;
        for (int c = 0; c < 3; ++c)
        {
            cv::Mat matPlane = nChannels[0][c];
            memcpy((float*)(pvData)+c * nPlaneSize,
            matPlane.data, nPlaneSize * sizeof(float));
        }
        auto nchwTensor = new Tensor(input, Tensor::CAFFE);
        ::memcpy(nchwTensor->host<float>(), pvData, size_w* size_h * 3 * sizeof(float));
        input->copyFromHostTensor(nchwTensor);
        delete nchwTensor;
        free(pvData);
        pvData = NULL;
    }
	
 
    net->runSession(session);


    Tensor* pTensorOutput = net->getSessionOutput(session, NULL);
    auto dimType = pTensorOutput->getDimensionType();
    if (pTensorOutput->getType().code != halide_type_float) {
        dimType = Tensor::CAFFE;
    }
    auto outputUser = new Tensor(pTensorOutput, Tensor::CAFFE);
 // 拷贝出去
    pTensorOutput->copyToHostTensor(outputUser);
    void* getData = malloc(1 * 1 * size_w * size_h * sizeof(float));
    ::memcpy(getData, outputUser->host<float>(), size_w * 1 * size_h * sizeof(float));
    cv::Mat out(size_w, size_w, CV_32F);
    std::memcpy(out.ptr<float>(0), getData, size_w * size_w * sizeof(float));
    cv::Mat matting_img =seg_postprocess(out, matBgrImg);
 
    free(getData);
    cv::Mat output_img =hollow_out_fix(matting_img);
    
    return output_img;
  
}
   



