#ifndef _HIVISON_SDK_H_
#define _HIVISON_SDK_H_

#include "main.h"

#ifdef _WIN32
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __attribute__((visibility("default")))
#endif


extern "C" DLL_API typedef struct Hivision_config

{

    char* model_path;
    char* image_path;
    char* out_path;
    char* face_model_path;
    int background_r=255;
    int background_g=0;
    int background_b=0;
    int thread_num=4;
    int model_scale=8;
    float head_measure_ratio = 0.35;
    int out_image_width = 295;
	int out_image_height = 413;
    

}Hivision_config;
extern "C" DLL_API int ID_photo (Hivision_config hivision,int out_size_kb=-1,bool layout_phot=false);
extern "C" DLL_API  void human_mating (Hivision_config hivision);

#endif