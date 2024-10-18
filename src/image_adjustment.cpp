#include "image_adjustment.h"




std::pair<int, int> detectDistance(int value, int cropHeight, double max = 0.06, double min = 0.04) {
    double ratio = static_cast<double>(value) / cropHeight;
    if (min <= ratio && ratio <= max) {
        return { 0, 0 };
    }
    else if (ratio > max) {
        int moveValue = static_cast<int>((ratio - max) * cropHeight);
        return { 1, moveValue };
    }
    else {
        int moveValue = static_cast<int>((min - ratio) * cropHeight);
        return { -1, moveValue };
    }
}




static cv::Mat IDphotos_cut(int x1, int y1, int x2, int y2, const cv::Mat& img_bgra, int rgb_r,int rgb_g ,int rgb_b) {

    cv::Mat img;
    cv::cvtColor(img_bgra, img, cv::COLOR_BGR2BGRA);
    int cropWidth = x2 - x1;
    int cropHeight = y2 - y1;

    int tempX1 = 0;
    int tempY1 = 0;
    int tempX2 = 0;
    int tempY2 = 0;

    if (y1 < 0) {
        tempY1 = -y1;
        y1 = 0;
    }
    if (y2 > img.rows) {
        tempY2 = y2 - img.rows;

        y2 = img.rows;
    }

    if (x1 < 0) {
        tempX1 = -x1;
        x1 = 0;
    }
    if (x2 > img.cols) {
        tempX2 = x2 - img.cols;
        x2 = img.cols;
    }

        cv::Mat background(cropHeight, cropWidth, CV_8UC4, cv::Scalar(rgb_b, rgb_g, rgb_r, 255));




            img(cv::Range(y1, y2), cv::Range(x1, x2)).copyTo(background(cv::Range(tempY1, cropHeight - tempY2), cv::Range(tempX1, cropWidth - tempX2)));



        return background;


}

std::vector<int> getBox(const cv::Mat& image, int model = 1, int correctionFactor = 0, int thresh = 127) {
    // 输入必须为四通道
    if (image.channels() != 4) {
        throw std::invalid_argument("Image must be in 4 channels!");
    }
    // correction_factor 规范化
    std::vector<int> correction;
    if (correctionFactor == 0) {
        correction = { 0, 0, 0, 0 };
    }
    else if (correctionFactor > 0) {
        correction = { 0, 0, correctionFactor, correctionFactor };
    }
    else {
        correction = { correctionFactor, correctionFactor, correctionFactor, correctionFactor };
    }
    // 分离 mask
    std::vector<cv::Mat> channels;
    split(image, channels);
    cv::Mat mask = channels[3];
    // mask 二值化处理
    threshold(mask, mask, thresh, 255, 0);
    std::vector<std::vector<cv::Point>> contours;
    std::vector<cv::Vec4i> hierarchy;
    findContours(mask, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_SIMPLE);
    cv::Mat temp(image.size(), CV_8UC4, cv::Scalar(255, 255, 255, 255));
    drawContours(temp, contours, -1, cv::Scalar(0, 0, 255, 255), -1);
    std::vector<int> contoursArea;
    for (const auto& cnt : contours) {
        contoursArea.push_back(contourArea(cnt));
    }
    auto idx = std::max_element(contoursArea.begin(), contoursArea.end()) - contoursArea.begin();
    cv::Rect rect = boundingRect(contours[idx]);
    int x = rect.x;
    int y = rect.y;
    int w = rect.width;
    int h = rect.height;
    cv::Mat imageWithBox = image.clone();
    cv::rectangle(imageWithBox, cv::Point(x, y), cv::Point(x + w, y + w), cv::Scalar(0, 255, 0), 2);

    // 显示带有框的图像
    int height = image.rows;
    int width = image.cols;
    int y_up = y - correction[0] >= 0 ? y - correction[0] : 0;
    int y_down = y + h + correction[1] < height ? y + h + correction[1] : height - 1;
    int x_left = x - correction[2] >= 0 ? x - correction[2] : 0;
    int x_right = x + w + correction[3] < width ? x + w + correction[3] : width - 1;
    if (model == 1) {
        return { y_up, y_down, x_left, x_right };
    }
    else if (model == 2) {

        return { y_up, height - y_down, x_left, width - x_right };
    }
    else {
        throw std::runtime_error("请选择正确的模式！");
    }
}


cv::Mat photo_adjust(matting_params params, cv::Mat adjust_photo, int out_image_height,int out_image_width,int rgb_r,int rgb_g,int rgb_b, float head_measure_ratio) {

    cv::Mat result_image;


    float x1 = params.face_info.x1;
    float x2 = params.face_info.x2;
    float y1 = params.face_info.y1;
    float y2 = params.face_info.y2;

    float face_width = x2 - x1;
    float face_height = y2 - y1;

    // 计算面部中心和裁剪框
    cv::Point2f face_center(x1 + (face_width / 2), y1 + (face_height / 2));

    int  width_height_ratio = out_image_width / out_image_height;
    float crop_measure = (face_width * face_height) / head_measure_ratio;
    float resize_ratio = crop_measure / (out_image_height * out_image_width);
    float  resize_ratio_single = sqrt(
        resize_ratio
    );
    int crop_size_x = out_image_width * resize_ratio_single;
    int crop_size_y = out_image_height * resize_ratio_single;
    int crop_x1 = std::lround(face_center.x - crop_size_x / 2);
    int crop_y1 = std::lround(face_center.y - crop_size_y * params.head_height_ratio);
    int crop_y2 = crop_y1 + crop_size_y;
    int crop_x2 = crop_x1 + crop_size_x;

    cv::Mat cut_image = IDphotos_cut(crop_x1, crop_y1, crop_x2, crop_y2, adjust_photo, rgb_r,rgb_g,rgb_b);


    cv::Mat resize_cut_img;
    cv::Size crop_size(crop_size_x, crop_size_y);
    cv::resize(cut_image, resize_cut_img, crop_size);
    std::vector<int> distances = getBox(cut_image, 2, 0);

    int yTop = distances[0];
    int yBottom = distances[1];
    int xLeft = distances[2];
    int xRight = distances[3];

    // 检测人像与裁剪框左边或右边是否存在空隙
    int statusLeftRight = xLeft > 0 || xRight > 0 ? 1 : 0;
    int cutValueTop = statusLeftRight == 1 ? ((xLeft + xRight) * width_height_ratio) / 2 : 0;
    std::pair<int, int> statusTopMoveValue = detectDistance(yTop - cutValueTop, crop_size_y);

    int statusTop = statusTopMoveValue.first;
    int moveValue = statusTopMoveValue.second;
    if (statusLeftRight == 0 && statusTop == 0) {
        result_image = cut_image;
    }
    else {
        result_image = IDphotos_cut(
            crop_x1 + xLeft,
            crop_y1 + cutValueTop + statusTop * moveValue,
            crop_x2 - xRight,
            crop_y2 - cutValueTop + statusTop * moveValue,
            adjust_photo,
            rgb_r,rgb_g,rgb_b
        );
    }




    return result_image;
}
