#pragma once
#ifndef _IMAGE_ADJUSTMENT_H_
#define _IMAGE_ADJUSTMENT_H_

#include "main.h"
#include <cmath> // 包含cmath库以使用sqrt函数


struct FaceInfo; // 前向声明
cv::Mat  photo_adjust(Params params, cv::Mat adjust_photo);

//typedef struct Params {
//	int out_image_width = 413;
//	int out_image_height = 295;
//	bool change_bg_only = false;
//	float head_measure_ratio = 0.2;
//	float head_height_ratio = 0.45;
//	float head_top_range[2] = {0.12f, 0.1f};
//	FaceInfo face_info;
//
//}Params;






#endif // !_IMAGE_ADJUST_H
