#ifndef _HIVISON_JAVA_H_
#define _HIVISON_JAVA_H_

#include "main.h"




typedef struct Hivision_java_params

{

    std::string model_path;
    std::string image_path;
    std::string out_path;
    std::string face_model_path;
    int rgb_r=255;
    int rgb_g=0;
    int rgb_b=0;
    int thread_num=4;
    int model_scale=8;
    Params param;
    /* data */
}Hivision_java_params;



#endif