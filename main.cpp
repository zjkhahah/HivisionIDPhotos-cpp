#include "main.h"




Params params;

int main(int argc, char* argv[]) {

	cmdline::parser p;
	p.add<std::string>("input_image", 'i', "Enter image path", false, "./demo/images/test2.jpg");
	p.add<std::string>("output_image", 'o', "Enter image path", false, ".");
	p.add<int>("out_size_kb", 'k', "image size", false, 20);
	p.add<int>("thread_num", 't', "model use thread num", false, 4);
	p.add<int>("background_color_r", 'r', "background red", false, 255);
	p.add<int>("background_color_g", 'g', "background green", false, 0);
	p.add<int>("background_color_b", 'b', "background blue", false, 0);
	p.add<int>("out_images_width", 'w', "out images width", false, 295);
	p.add<int>("out_images_height", 'h', "out images height", false, 413);
	p.add<int>("face_model", 'f', "face_model type 5 and 8", false, 8);
	std::string face_model_path = "./model";
	const char* segment_modnet = "./model/mnn_hivision_modnet.mnn";
	p.parse_check(argc, argv);

	params.out_image_width=p.get<int>("out_images_width");
	params.out_image_height=p.get<int>("out_images_height");
	params.rgb_b=p.get<int>("background_color_b");
	params.rgb_g=p.get<int>("background_color_g");
	params.rgb_r=p.get<int>("background_color_r");
	
	cv::Vec3b newBackgroundColor(p.get<int>("background_color_b"), p.get<int>("background_color_g"), p.get<int>("background_color_r"));
	LFFD* face_detector = new LFFD(face_model_path, p.get<int>("face_model"), p.get<int>("thread_num"));

	cv::Mat image = cv::imread(p.get<std::string>("input_image"), cv::IMREAD_COLOR);
	cv::Mat bgra_img=	Interference(segment_modnet, image,4);

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
		params.face_info = finalBox[0];
	}
	free(face_detector);
	cv::Mat hd_result= photo_adjust(params, add_background_img);
	resizeImageToKB(hd_result, p.get < std::string>("output_image")+"result_kb.png",p.get <int>("out_size_kb") );
	cv::Mat standard_result;
	cv::Size standard_size(params.out_image_width, params.out_image_height);
	cv::resize(hd_result, standard_result, standard_size);
	cv::imwrite(p.get < std::string>("output_image") +"result_hd.png", hd_result);
	cv::imwrite(p.get < std::string>("output_image")+"result_standard.png", standard_result);

	return 0;
}