#include "Hivision_Java.h"







void human_mating (Hivision_java_params hivision){

    std::string modelPathStr = hivision.model_path;
    const char* modelPath = modelPathStr.c_str();
	cv::Mat image =cv::imread(modelPath, cv::IMREAD_COLOR);
    cv::Mat bgra_img=	Interference(modelPath, image,4);
    cv::imwrite(hivision.out_path+"human_matting.png", bgra_img);

}

int ID_photo (Hivision_java_params hivision,int out_size_kb,bool layout_phot){


	cv::Vec3b newBackgroundColor(hivision.rgb_b,hivision.rgb_g,hivision.rgb_r);
    LFFD* face_detector = new LFFD(hivision.face_model_path,hivision.model_scale ,hivision.thread_num );
	cv::Mat image =cv::imread(hivision.image_path, cv::IMREAD_COLOR);
    std::string modelPathStr = hivision.model_path;
    const char* modelPath = modelPathStr.c_str();
	cv::Mat bgra_img=	Interference(modelPath, image,4);
    cv::Mat add_background_img = addBackground(bgra_img, newBackgroundColor);
	cv::cvtColor(add_background_img, add_background_img, cv::COLOR_BGRA2BGR);

	std::vector<FaceInfo > finalBox;
	face_detector->detect(add_background_img, finalBox, add_background_img.rows, add_background_img.cols);
	for (int i = 0; i < finalBox.size(); i++) {
		FaceInfo facebox = finalBox[i];
		// cv::Rect box = cv::Rect(facebox.x1, facebox.y1, facebox.x2 - facebox.x1, facebox.y2 - facebox.y1);
		// cv::rectangle(add_background_img, box, cv::Scalar(255, 0, 21), 2);
	}
	if (finalBox.size() > 1) {
		printf("输入人脸不为 1");
		return 0;
	}
	else {
		hivision.param.face_info = finalBox[0];
	}
	free(face_detector);
	cv::Mat hd_result= photo_adjust(hivision.param, add_background_img);
    cv::Mat standard_result;
	cv::Size standard_size(hivision.param.out_image_width, hivision.param.out_image_height);
    cv::resize(hd_result, standard_result, standard_size);

	if(out_size_kb>0){
        std::string modelPathStr = hivision.model_path;
		resizeImageToKB(hd_result,+"result_kb.png",out_size_kb);
	}
	
    if(layout_phot){
		auto result_typography_arr =generate_layout_photo(hivision.param.out_image_height, hivision.param.out_image_width);
		cv::Mat result_layout_image = generate_layout_image(
		standard_result,
		std::get<0>(result_typography_arr),
		std::get<1>(result_typography_arr),
		hivision.param.out_image_width,
		hivision.param.out_image_height
		);
		cv::imwrite( hivision.out_path+"layout_photo.png", result_layout_image);
	 }
	cv::imwrite( hivision.out_path+"result_hd.png", hd_result);
	cv::imwrite(hivision.out_path+"result_standard.png", standard_result);

   
}