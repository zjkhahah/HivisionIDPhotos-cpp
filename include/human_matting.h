#pragma once
#ifndef _INTERFENCE_H_
#define  _INTERFENCE_H_


#include "main.h"


using namespace MNN;

cv::Mat  human_matting(const char* & mnn_path, cv::Mat input_BgrImg,int num_thread) ;

#define SIZE_H  512;
#define SIZE_W  512;


#endif
