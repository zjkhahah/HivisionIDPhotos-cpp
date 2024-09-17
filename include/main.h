#pragma once
#ifndef _MAIN_H_
#define _MAIN_H_


struct FaceInfo; // 前向声明
struct Params;

#include <opencv2/opencv.hpp>
#include <MNN/Interpreter.hpp>
#include <MNN/MNNDefine.h>
#include <MNN/Tensor.hpp>
#include <MNN/ImageProcess.hpp>
#include "human_matting.h"
#include "MNN_LFFD.h"
#include "tool.h"
#include "image_adjustment.h"
#include <string>
#include <vector>
#include <algorithm>
#include <iostream>
#include <memory>
#include "cmdline.h"




#define 	OUT_SCALE_5   5
#define		OUT_SCALE_8   8






#endif
