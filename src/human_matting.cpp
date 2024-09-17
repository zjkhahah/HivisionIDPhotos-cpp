#include "human_matting.h"



static cv::Mat image3bgr(const cv::Mat& input_image) {
    cv::Mat result_image;

    if (input_image.channels() == 1) { // 灰度图（单通道）
        // 复制单通道图像到三个通道
        cv::cvtColor(input_image, result_image, cv::COLOR_GRAY2BGR);
    }
    else if (input_image.channels() == 2) { // 可能是单通道图像加上一个alpha通道
        // 复制两通道图像到三个通道
        cv::cvtColor(input_image, result_image, cv::COLOR_GRAY2BGR);
    }
    else if (input_image.channels() == 4) { // 四通道图像（可能是RGBA）
        // 从四通道图像中提取前三个通道（BGR）
        result_image = input_image(cv::Rect(0, 0, input_image.cols, input_image.rows));
        cv::cvtColor(result_image, result_image, cv::COLOR_RGBA2BGR);
    }
    else if (input_image.channels() == 3) { // 已经是BGR
        result_image = input_image.clone();
    }
    else {
        std::cerr << "Unsupported number of channels" << std::endl;
    }

    return result_image;
}


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




cv::Mat  Interference(const char* & mnn_path, cv::Mat input_BgrImg,int num_thread) {

 
    cv::Mat matBgrImg =image3bgr(input_BgrImg);//设定图片通道

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
    int size_w = 512;  // 初始化宽度
    int size_h = 512;  // 初试化高度
    auto shape = input->shape();
    if (shape[0] != 1) {
        shape[0] = 1;
        net->resizeTensor(input, shape);
        net->resizeSession(session);
    }
    {
        int bpp = 0;
        bpp = shape[1];
        size_h = shape[2];
        size_w = shape[3];
        if (bpp == 0)
            bpp = 1;
        if (size_h == 0)
            size_h = 1;
        if (size_w == 0)
            size_w = 1;
        MNN_PRINT("input: w:%d , h:%d, bpp: %d\n", size_w, size_h, bpp);
    }

    cv::Mat pre_img = seg_preprocess( matBgrImg,size_w, size_h);
    std::vector<std::vector<cv::Mat>> nChannels;
    std::vector<cv::Mat> rgbChannels(3);
    cv::split(pre_img, rgbChannels);
    nChannels.push_back(rgbChannels); //  NHWC  转NCHW
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
    net->runSession(session);
    Tensor* pTensorOutput = net->getSessionOutput(session, NULL);
    auto dimType = pTensorOutput->getDimensionType();
    if (pTensorOutput->getType().code != halide_type_float) {
        dimType = Tensor::CAFFE;
    }
    auto outputUser = new Tensor(pTensorOutput, Tensor::CAFFE);
    MNN_PRINT("output size:%d\n", outputUser->elementSize());
 // 拷贝出去
    pTensorOutput->copyToHostTensor(outputUser);
    void* getData = malloc(1 * 1 * size_w * size_h * sizeof(float));
    ::memcpy(getData, outputUser->host<float>(), size_w * 1 * size_h * sizeof(float));
    cv::Mat out(size_w, size_w, CV_32F);
    std::memcpy(out.ptr<float>(0), getData, size_w * size_w * sizeof(float));
    cv::Mat output_img =seg_postprocess(out, matBgrImg);
    free(getData);
    return output_img;
  
}
   

//int main() {
//    const char* mnn_path = "F:\\HivisionIDPhotos\\HivisionIDPhotos\\HivisionIDPhotos\\mnn_hivision_modnet.mnn";
//    const char* inputPatch = "F:\\HivisionIDPhotos\\HivisionIDPhotos\\HivisionIDPhotos\\test.jpg";
//    cv::Mat resultImage = Interference(mnn_path, inputPatch);
//    cv::imwrite("testzjk.png", resultImage);
//    return 0;
//}


