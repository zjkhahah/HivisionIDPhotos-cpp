#pragma once
#ifndef _TOOL_H_
#define _TOOL_H_

#include "main.h"
#include <fstream>

cv::Mat addBackground(const cv::Mat& input_image, const cv::Vec3b& bgr = cv::Vec3b(0, 0, 0), const std::string& mode = "pure_color");
bool resizeImageToKB(const cv::Mat& inputImage, const std::string& outputImagePath, int targetSizeKB);






#endif // !1
