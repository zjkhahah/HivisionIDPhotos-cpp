#pragma once
#ifndef _INTERFENCE_H_
#define  _INTERFENCE_H_


#include "main.h"


using namespace MNN;

cv::Mat  Interference(const char*& mnn_path, cv::Mat input_BgrImg, int num_thread);


#endif
