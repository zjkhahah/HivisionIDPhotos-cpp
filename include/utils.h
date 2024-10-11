#pragma once
#ifndef _UTILS_H_
#define _UTILS_H_

#include "main.h"
#include <fstream>


cv::Mat image3bgr(const cv::Mat& input_image) ;
cv::Mat addBackground(const cv::Mat& input_image, const cv::Vec3b& bgr = cv::Vec3b(0, 0, 0), const std::string& mode = "pure_color");
bool resizeImageToKB(const cv::Mat& inputImage, const std::string& outputImagePath, int targetSizeKB);
std::pair<std::vector<std::vector<int>>, bool> generate_layout_photo(int input_height, int input_width);
cv::Mat generate_layout_image(
    const cv::Mat& input_image,
    const std::vector<std::vector<int>>& typography_arr,
    bool typography_rotate,
    int width = 295,
    int height = 413
);
cv::Mat add_padding(const cv::Mat& src, int pad_size, int borderType, const cv::Scalar& value);
bool compareContours(const std::vector<cv::Point>& contour1, const std::vector<cv::Point>& contour2);

#endif // !1
