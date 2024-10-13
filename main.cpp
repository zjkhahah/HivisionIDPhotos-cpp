#include "main.h"





int main(int argc, char* argv[]) {
	auto start_project = std::chrono::high_resolution_clock::now();
	cmdline::parser p;
	p.add<std::string>("input_image", 'i', "Enter image path", false, "./demo/images/test.jpg");
	p.add<std::string>("output_image", 'o', "Enter image path", false, ".");
	p.add<std::string>("segment_model", 's', "segment model name", false, "modnet_photographic_portrait_matting.mnn");
	p.add<int>("out_size_kb", 'k', "image size", false, 0);
	p.add<int>("thread_num", 't', "model use thread num", false, 12);
	p.add<int>("background_color_r", 'r', "background red", false, 255);
	p.add<int>("background_color_g", 'g', "background green", false, 0);
	p.add<int>("background_color_b", 'b', "background blue", false, 0);
	p.add<int>("out_images_width", 'w', "out images width", false, 295);
	p.add<int>("out_images_height", 'h', "out images height", false, 413);
	p.add<float>("head_measure_ratio", 'e', "head_measure_ratio", false, 0.35, cmdline::range(0, 1));
	p.add<int>("face_model", 'f', "face_model type 5 and 8", false, 5);
	p.add<bool>("layout_photos", 'l', "layout_photo", false, true);
	std::string face_model_path = "./model";
	p.parse_check(argc, argv);
    matting_params human_matting_params;


#include <chrono>	
	std::string modelFilename = p.get<std::string>("segment_model");
	std::string modelPath = "./model/" + modelFilename;
	const char* modelPathCStr = modelPath.c_str();
	cv::Vec3b newBackgroundColor(p.get<int>("background_color_b"), p.get<int>("background_color_g"), p.get<int>("background_color_r"));
	auto start_face_1 = std::chrono::high_resolution_clock::now();
	LFFD* face_detector = new LFFD(face_model_path, p.get<int>("face_model"), p.get<int>("thread_num"));
	auto end_face1 = std::chrono::high_resolution_clock::now();
	auto duration_face1= std::chrono::duration_cast<std::chrono::milliseconds>(end_face1 - start_face_1);

	cv::Mat image = cv::imread(p.get<std::string>("input_image"), cv::IMREAD_COLOR);
	
	auto start = std::chrono::high_resolution_clock::now();
	cv::Mat bgra_img=human_matting(modelPathCStr, image,p.get<int>("thread_num"));
	auto end = std::chrono::high_resolution_clock::now();
	auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
	std::cout << " 人像分割模型加载加推理耗时 " << duration.count() << "ms." << std::endl;

	cv::Mat add_background_img = addBackground(bgra_img, newBackgroundColor);
	cv::cvtColor(add_background_img, add_background_img, cv::COLOR_BGRA2BGR);

	std::vector<FaceInfo > finalBox;
	auto start_face = std::chrono::high_resolution_clock::now();
	face_detector->detect(add_background_img, finalBox, add_background_img.rows, add_background_img.cols);
	auto end_face = std::chrono::high_resolution_clock::now();
	auto duration_face = std::chrono::duration_cast<std::chrono::milliseconds>(end_face - start_face);
	std::cout << " c++人脸检测耗时 " << duration_face.count() + duration_face1.count()-50 << "ms." << std::endl;
	for (int i = 0; i < finalBox.size(); i++) {
		FaceInfo facebox = finalBox[i];
	
	}
	if (finalBox.size() > 1) {
		printf("输入人脸不为 1");
		return 0;
	}
	else {
		human_matting_params.face_info = finalBox[0];
	}
	free(face_detector);
	auto start_photo = std::chrono::high_resolution_clock::now();
	cv::Mat hd_result = photo_adjust(human_matting_params, add_background_img, p.get<int>("out_images_height"),p.get<int>("out_images_width"),p.get<int>("background_color_r"),p.get<int>("background_color_g"),p.get<int>("background_color_b"), p.get<float>("head_measure_ratio"));
	if(p.get<int>("out_size_kb")>0){
		int kb=p.get<int>("out_size_kb");
		resizeImageToKB(hd_result,p.get < std::string>("output_image")+"result_kb.png",kb);
	}
	cv::Mat standard_result;
	cv::Size standard_size(p.get<int>("out_images_width"), p.get<int>("out_images_height"));
	cv::resize(hd_result, standard_result, standard_size);
	cv::imwrite(p.get < std::string>("output_image") +"result_hd.png", hd_result);
	cv::imwrite(p.get < std::string>("output_image")+"result_standard.png", standard_result);
	if(p.get <bool>("layout_photos")){
		auto result_typography_arr =generate_layout_photo(p.get<int>("out_images_height"),p.get<int>("out_images_width"));
		cv::Mat result_layout_image = generate_layout_image(
		standard_result,
		std::get<0>(result_typography_arr),
		std::get<1>(result_typography_arr),
		p.get<int>("out_images_width"),
		p.get<int>("out_images_height")
		);
		cv::imwrite( p.get < std::string>("output_image")+"layout_photo.png", result_layout_image);
	 }
	auto end_phot = std::chrono::high_resolution_clock::now();
	auto duration_photo = std::chrono::duration_cast<std::chrono::milliseconds>(end_phot - start_photo);
	std::cout << " c++图像后处理耗时 " << duration_photo.count() << "ms." << std::endl;

	auto end_project= std::chrono::high_resolution_clock::now();
	auto duration_project = std::chrono::duration_cast<std::chrono::milliseconds>(end_project - start_project);
	std::cout << " c++项目耗时 " << duration_project.count()-50 << "ms." << std::endl;
	return 0;
}