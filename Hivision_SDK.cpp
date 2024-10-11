#include "HIvision_SDK.h"





void human_mating (Hivision_config hivision){
	std::string ImagePathStr(hivision.image_path);
	std::string ModelPathStr(hivision.model_path);
	std::string OutPathStr(hivision.out_path);

    const char* ImagePath = ImagePathStr.c_str();
	const char* ModelPath = ModelPathStr.c_str();
	

	cv::Mat image =cv::imread(ImagePath, cv::IMREAD_COLOR);
    cv::Mat bgra_img=human_matting(ModelPath,image,4);
    cv::imwrite(OutPathStr+"human_matting.png", bgra_img);

}



int ID_photo (Hivision_config hivision,int out_size_kb,bool layout_phot){

    matting_params human_matting_params;
	std::string ImagePathStr(hivision.image_path);
	std::string ModelPathStr(hivision.model_path);
	std::string OutPathStr(hivision.out_path);
	std::string FaceModelPathStr(hivision.face_model_path);

    const char* ImagePath = ImagePathStr.c_str();
	const char* ModelPath = ModelPathStr.c_str();

	cv::Vec3b newBackgroundColor(hivision.background_b,hivision.background_g,hivision.background_r);
    LFFD* face_detector = new LFFD(FaceModelPathStr,hivision.model_scale ,hivision.thread_num );
	cv::Mat image =cv::imread(ImagePath, cv::IMREAD_COLOR);

	cv::Mat bgra_img=human_matting(ModelPath, image,4);
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
		human_matting_params.face_info = finalBox[0];
	}
	free(face_detector);
    cv::Mat hd_result = photo_adjust(human_matting_params, add_background_img, hivision.out_image_height,hivision.out_image_width,hivision.background_r,hivision.background_g,hivision.background_b, hivision.head_measure_ratio);
    cv::Mat standard_result;
	cv::Size standard_size(hivision.out_image_width,hivision.out_image_height);
    cv::resize(hd_result, standard_result, standard_size);

	if(out_size_kb>0){
    
		resizeImageToKB(hd_result,OutPathStr+"result_kb.png",out_size_kb);
	}
	
    if(layout_phot){
		auto result_typography_arr =generate_layout_photo(hivision.out_image_height,hivision.out_image_width);
		cv::Mat result_layout_image = generate_layout_image(
		standard_result,
		std::get<0>(result_typography_arr),
		std::get<1>(result_typography_arr),
		hivision.out_image_width,
		hivision.out_image_height
		);
		cv::imwrite( OutPathStr+"layout_photo.png", result_layout_image);
	 }
	cv::imwrite( OutPathStr+"result_hd.png", hd_result);
	cv::imwrite(OutPathStr+"result_standard.png", standard_result);
	return 1;
   
}