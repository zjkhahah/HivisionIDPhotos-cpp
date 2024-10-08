#include "main.h"



FaceSDKConfig getConfig(int w, int h)
{
    FaceSDKConfig config;
    config.img_w = w;
    config.img_h = h;
    config.screen_w = w;
    config.screen_h = h;
    config.input_format = ImageFormat::BGR;
    config.mode = FaceSDKMode::Normal;
    config.thread_num = 2;
    return config;
}
Params params;

int main(int argc, char* argv[]) {
	float eye_close_v = 0.5f;
	cmdline::parser p;
	p.add<std::string>("input_image", 'i', "Enter image path", false, "./demo/images/test2.jpg");
	p.add<std::string>("output_image", 'o', "Enter image path", false, ".");
	p.add<std::string>("segment_model", 's', "segment model name", false, "mnn_hivision_modnet.mnn");
	p.add<int>("out_size_kb", 'k', "image size", false, 0);
	p.add<int>("thread_num", 't', "model use thread num", false, 4);
	p.add<int>("background_color_r", 'r', "background red", false, 255);
	p.add<int>("background_color_g", 'g', "background green", false, 0);
	p.add<int>("background_color_b", 'b', "background blue", false, 0);
	p.add<int>("out_images_width", 'w', "out images width", false, 295);
	p.add<int>("out_images_height", 'h', "out images height", false, 413);
	p.add<int>("face_model", 'f', "face_model type 5 and 8", false, 8);
	std::string face_model_path = "./model";
	//const char* segment_modnet = "./model/mnn_hivision_modnet.mnn";
	p.parse_check(argc, argv);

	params.out_image_width=p.get<int>("out_images_width");
	params.out_image_height=p.get<int>("out_images_height");
	params.rgb_b=p.get<int>("background_color_b");
	params.rgb_g=p.get<int>("background_color_g");
	params.rgb_r=p.get<int>("background_color_r");

	std::string modelFilename = p.get<std::string>("segment_model");
	std::string modelPath = "./model/" + modelFilename;
	const char* modelPathCStr = modelPath.c_str();
	cv::Vec3b newBackgroundColor(p.get<int>("background_color_b"), p.get<int>("background_color_g"), p.get<int>("background_color_r"));
	// LFFD* face_detector = new LFFD(face_model_path, p.get<int>("face_model"), p.get<int>("thread_num"));

	cv::Mat image = cv::imread(p.get<std::string>("input_image"), cv::IMREAD_COLOR);
	cv::Mat bgra_img=	Interference(modelPathCStr, image,4);

	cv::Mat add_background_img = addBackground(bgra_img, newBackgroundColor);
	cv::cvtColor(add_background_img, add_background_img, cv::COLOR_BGRA2BGR);
	int w = add_background_img.cols;
    int h = add_background_img.rows;
	FaceSDKConfig config = getConfig(w, h);
	facesdk_init(config);
	char data[w * h * 3];
    memcpy(data, (char *)add_background_img.data, w * h * 3);
    facesdk_readModelFromFile(ModelType::Detect, "./model/face_detect.bin", ImageFormat::RGB);
    sdkFaces faces = facesdk_detect(data);
	 std::cout << "faces:" << faces.face_count << std::endl;
    for (int i = 0; i < faces.face_count; i++)
    {
        cv::Point pt1(faces.info[i].face_box.x1, faces.info[i].face_box.y1);
        cv::Point pt2(faces.info[i].face_box.x2, faces.info[i].face_box.y2);
        cv::rectangle(add_background_img, pt1, pt2, cv::Scalar(255, 0, 0), 2);
    }
	    facesdk_readModelFromFile(ModelType::Landmark, "./model/yolov5s.bin", ImageFormat::RGB);
    sdkFaces faces2 = facesdk_landmark();
    int index = 0;
    for (int j = 0; j < faces2.face_count; j++)
    {
        index++;
        for (int i = 0; i < 424; i = i + 2)
        {
            cv::Point pt(faces2.info[j].landmarks[i], faces2.info[j].landmarks[i + 1]);
            cv::circle(add_background_img, pt, 1, cv::Scalar(255, 0, 0), 1);
        }
        std::cout << std::endl;

        if (faces2.info[j].lefteye_close_state < eye_close_v)
        {
            std::cout << "左眼开" << std::endl;
            cv::putText(add_background_img, "left eye open",
                        cv::Point(10, 20 + index * 30), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 255, 255), 2);
        }
        else
        {
            std::cout << "左眼关" << std::endl;
            cv::putText(add_background_img, "left eye close",
                        cv::Point(10, 20 + index * 30), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 255, 255), 2);
        }

        if (faces2.info[j].righteye_close_state < eye_close_v)
        {
            std::cout << "右眼开" << std::endl;
            cv::putText(add_background_img, "right eye open",
                        cv::Point(10, 50 + index * 30), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 255, 255), 2);
        }
        else
        {
            std::cout << "右眼关" << std::endl;
            cv::putText(add_background_img, "right eye close",
                        cv::Point(10, 50 + index * 30), cv::FONT_HERSHEY_DUPLEX, 1, cv::Scalar(0, 255, 255), 2);
        }
        std::cout << std::endl;
    }
	facesdk_readModelFromFile(ModelType::Attribution, "./model/face_attr.bin", ImageFormat::RGB);
    sdkFaces faces3 = facesdk_attribute();
    std::cout << faces3.face_count << std::endl;
    for (int i = 0; i < faces3.face_count; i++)
    {
        std::cout << "age: " << faces3.info[i].attribution.age << std::endl;
        std::cout << "gender: " << faces3.info[i].attribution.gender << std::endl;
        std::cout << "glasses: " << faces3.info[i].attribution.glasses << std::endl;
        std::cout << "smile: " << faces3.info[i].attribution.smile << std::endl;
        std::cout << "beauty_man_look: " << faces3.info[i].attribution.beauty_man_look << std::endl;
        std::cout << "beauty_woman_look: " << faces3.info[i].attribution.beauty_woman_look << std::endl;
    }
	cv::imwrite("output/output.jpg", add_background_img);
    facesdk_release();
	// std::vector<FaceInfo > finalBox;
	// face_detector->detect(add_background_img, finalBox, add_background_img.rows, add_background_img.cols);
	// for (int i = 0; i < finalBox.size(); i++) {
	// 	FaceInfo facebox = finalBox[i];
	// 	// cv::Rect box = cv::Rect(facebox.x1, facebox.y1, facebox.x2 - facebox.x1, facebox.y2 - facebox.y1);
	// 	// cv::rectangle(add_background_img, box, cv::Scalar(255, 0, 21), 2);
	// }
	// if (finalBox.size() > 1) {
	// 	printf("输入人脸不为 1");
	// 	return 0;
	// }
	// else {
	// 	params.face_info = finalBox[0];
	// }
	// free(face_detector);
	// cv::Mat hd_result= photo_adjust(params, add_background_img);
	// if(p.get<int>("out_size_kb")>0){
	// 	resizeImageToKB(hd_result, p.get < std::string>("output_image")+"result_kb.png",p.get <int>("out_size_kb") );
	// }
	// cv::Mat standard_result;
	// cv::Size standard_size(params.out_image_width, params.out_image_height);
	// cv::resize(hd_result, standard_result, standard_size);
	// cv::imwrite(p.get < std::string>("output_image") +"result_hd.png", hd_result);
	// cv::imwrite(p.get < std::string>("output_image")+"result_standard.png", standard_result);

	return 0;
}