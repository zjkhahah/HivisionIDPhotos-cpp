#pragma once
#ifndef _IMAGE_ADJUSTMENT_H_
#define _IMAGE_ADJUSTMENT_H_

#include "main.h"
#include <cmath> // 包含cmath库以使用sqrt函数



cv::Mat photo_adjust(matting_params params, cv::Mat adjust_photo, int out_image_height,int out_image_width,int rgb_r,int rgb_g,int rgb_b, float head_measure_ratio);







#endif // !_IMAGE_ADJUST_H
