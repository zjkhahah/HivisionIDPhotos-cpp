#include "main.h"


bool compareContours(const std::vector<cv::Point>& contour1, const std::vector<cv::Point>& contour2) {
    double area1 = cv::contourArea(contour1);
    double area2 = cv::contourArea(contour2);
    return area1 > area2; // 返回 true 如果第一个轮廓的面积大于第二个
}



cv::Mat add_padding(const cv::Mat& src, int pad_size, int borderType, const cv::Scalar& value) {
    CV_Assert(src.type() != CV_8S); // 确保图像类型不是CV_8S，因为copyMakeBorder不支援

    // 添加填充
    cv::Mat dst;
    cv::copyMakeBorder(src, dst, pad_size, pad_size, pad_size, pad_size, borderType, value);

    return dst;
}

cv::Mat image3bgr(const cv::Mat& input_image) {
    cv::Mat result_image;

    if (input_image.channels() == 1) { // 灰度图（单通道）
        // 复制单通道图像到三个通道
        cv::cvtColor(input_image, result_image, cv::COLOR_GRAY2BGR);
    }
    else if (input_image.channels() == 2) { // 可能是单通道图像加上一个alpha通道
        // 复制两通道图像到三个通道
        cv::cvtColor(input_image, result_image, cv::COLOR_GRAY2BGR);
    }
    else if (input_image.channels() == 4) { // 四通道图像（可能是RGBA）
        // 从四通道图像中提取前三个通道（BGR）
        result_image = input_image(cv::Rect(0, 0, input_image.cols, input_image.rows));
        cv::cvtColor(result_image, result_image, cv::COLOR_RGBA2BGR);
    }
    else if (input_image.channels() == 3) { // 已经是BGR
        result_image = input_image.clone();
    }
    else {
        std::cerr << "Unsupported number of channels" << std::endl;
    }

    return result_image;
}

//设置图像大小
bool resizeImageToKB(const cv::Mat& inputImage, const std::string& outputImagePath, int targetSizeKB) {

    // Ensure the image is in RGB format
    cv::Mat rgbImage;
    if (inputImage.channels() == 3) {
        rgbImage = inputImage.clone();
    } else if (inputImage.channels() == 4) {
        cv::cvtColor(inputImage, rgbImage, cv::COLOR_BGRA2BGR);
    } else {
        std::cerr << "Image is not in a supported format" << std::endl;
        return false;
    }

    int quality = 95; // Initial quality value
    bool resized = false;

    while (!resized) {
        std::vector<uchar> buffer;
        cv::imencode(".jpg", rgbImage, buffer, std::vector<int>{cv::IMWRITE_JPEG_QUALITY, quality});

        // Check the size of the image in KB
        double imageSizeKB = buffer.size() / 1024.0;

        if (imageSizeKB <= targetSizeKB) {
            // Write the buffer to the output file
            std::ofstream outFile(outputImagePath, std::ios::binary);
            outFile.write(reinterpret_cast<char*>(buffer.data()), buffer.size());
            resized = true;
        } else {
            quality -= 5;
            if (quality < 1) {
                quality = 1; // Ensure quality does not go below 1
            }
        }
    }

    return true;
}





std::tuple<std::tuple<int, int, int>, int, int> judge_layout(
    int input_width,
    int input_height,
    int PHOTO_INTERVAL_W,
    int PHOTO_INTERVAL_H,
    int LIMIT_BLOCK_W,
    int LIMIT_BLOCK_H
) {
    int centerBlockHeight_1 = input_height; // 不转置排列的中心区块高度
    int centerBlockWidth_1 = input_width;   // 不转置排列的中心区块宽度
    int centerBlockHeight_2 = input_width;  // 转置排列的中心区块高度
    int centerBlockWidth_2 = input_height;  // 转置排列的中心区块宽度

    // 1. 不转置排列的情况下：
    int layout_col_no_transpose = 0;  // 列
    int layout_row_no_transpose = 0;  // 行
    for (int i = 1; i < 4; ++i) {
        int centerBlockHeight_temp = input_height * i + PHOTO_INTERVAL_H * (i - 1);
        if (centerBlockHeight_temp < LIMIT_BLOCK_H) {
            centerBlockHeight_1 = centerBlockHeight_temp;
            layout_row_no_transpose = i;
        }
        else {
            break;
        }
    }
    for (int j = 1; j < 9; ++j) {
        int centerBlockWidth_temp = input_width * j + PHOTO_INTERVAL_W * (j - 1);
        if (centerBlockWidth_temp < LIMIT_BLOCK_W) {
            centerBlockWidth_1 = centerBlockWidth_temp;
            layout_col_no_transpose = j;
        }
        else {
            break;
        }
    }
    int layout_number_no_transpose = layout_row_no_transpose * layout_col_no_transpose;

    // 2. 转置排列的情况下：
    int layout_col_transpose = 0;  // 列
    int layout_row_transpose = 0;  // 行
    for (int i = 1; i < 4; ++i) {
        int centerBlockHeight_temp = input_width * i + PHOTO_INTERVAL_H * (i - 1);
        if (centerBlockHeight_temp < LIMIT_BLOCK_H) {
            centerBlockHeight_2 = centerBlockHeight_temp;
            layout_row_transpose = i;
        }
        else {
            break;
        }
    }
    for (int j = 1; j < 9; ++j) {
        int centerBlockWidth_temp = input_height * j + PHOTO_INTERVAL_W * (j - 1);
        if (centerBlockWidth_temp < LIMIT_BLOCK_W) {
            centerBlockWidth_2 = centerBlockWidth_temp;
            layout_col_transpose = j;
        }
        else {
            break;
        }
    }
    int layout_number_transpose = layout_row_transpose * layout_col_transpose;

    // 返回结果
    if (layout_number_transpose > layout_number_no_transpose) {
        return std::make_tuple(std::make_tuple(layout_col_transpose, layout_row_transpose, 2), centerBlockWidth_2, centerBlockHeight_2);
    }
    else {
        return std::make_tuple(std::make_tuple(layout_col_no_transpose, layout_row_no_transpose, 1), centerBlockWidth_1, centerBlockHeight_1);
    }
}




std::pair<std::vector<std::vector<int>>, bool> generate_layout_photo(int input_height, int input_width) {
    // 基础参数
    const int LAYOUT_WIDTH = 1746;
    const int LAYOUT_HEIGHT = 1180;
    const int PHOTO_INTERVAL_H = 30;
    const int PHOTO_INTERVAL_W = 30;
    const int SIDES_INTERVAL_H = 50;
    const int SIDES_INTERVAL_W = 70;
    const int LIMIT_BLOCK_W = LAYOUT_WIDTH - 2 * SIDES_INTERVAL_W;
    const int LIMIT_BLOCK_H = LAYOUT_HEIGHT - 2 * SIDES_INTERVAL_H;

    // 创建一个 1180x1746 的空白画布
    cv::Mat white_background(cv::Size(LAYOUT_WIDTH, LAYOUT_HEIGHT), CV_8UC3, cv::Scalar(255, 255, 255));

    // 计算照片的 layout
    auto  result = judge_layout(input_width, input_height, PHOTO_INTERVAL_W, PHOTO_INTERVAL_H, LIMIT_BLOCK_W, LIMIT_BLOCK_H);
    auto layout_mode = std::get<0>(result);
    int centerBlockWidth = std::get<1>(result);
    int centerBlockHeight = std::get<2>(result);
    // 开始排列组合
    int x11 = (LAYOUT_WIDTH - centerBlockWidth) / 2;
    int y11 = (LAYOUT_HEIGHT - centerBlockHeight) / 2;
    std::vector<std::vector<int>> typography_arr;
    bool typography_rotate = (std::get<2>(layout_mode) == 2);

    // 根据是否转置调整输入宽度和高度
    int effective_width = typography_rotate ? input_height : input_width;
    int effective_height = typography_rotate ? input_width : input_height;

    for (int j = 0; j < std::get<1>(layout_mode); ++j) {
        for (int i = 0; i < std::get<0>(layout_mode); ++i) {
            int xi = x11 + i * effective_width + i * PHOTO_INTERVAL_W;
            int yi = y11 + j * effective_height + j * PHOTO_INTERVAL_H;
            typography_arr.push_back({ xi, yi });
        }
    }

    return { typography_arr, typography_rotate };
}




cv::Mat generate_layout_image(
    const cv::Mat& input_image,
    const std::vector<std::vector<int>>& typography_arr,
    bool typography_rotate,
    int width ,
    int height
) {
    // 定义布局的宽度和高度
    const int LAYOUT_WIDTH = 1746;
    const int LAYOUT_HEIGHT = 1180;
    cv::Mat resize_input_image;
    int type = input_image.type();
    int colos = input_image.channels();
    printf("type=%d colos=%d\n", type, colos);
   
    // 创建一个空白背景图像
    cv::Mat white_background(cv::Size(LAYOUT_WIDTH, LAYOUT_HEIGHT), CV_8UC3, cv::Scalar(255, 255, 255));
    printf("h=%d,(input_image.rows=%d\n", height, input_image.rows);
    // 检查输入图像的高度是否与预期的高度不同，如果是，则调整大小
    if (input_image.rows != height) {
  
        cv::resize(input_image, resize_input_image, cv::Size(width, height));
    }
    else {
        resize_input_image = input_image.clone();
        cv::cvtColor(resize_input_image, resize_input_image, cv::COLOR_BGRA2BGR);
    }
     //如果需要旋转图像，则进行旋转
    if (typography_rotate) {
        cv::transpose(resize_input_image, resize_input_image);
        std::swap(width, height); // 交换宽度和高度
    }

    // 遍历所有的排版坐标
    for (const auto& arr : typography_arr) {
        int locate_x = arr[0];
        int locate_y = arr[1];

        // 将输入图像复制到背景图像的指定位置
        cv::Rect roi(locate_x, locate_y, width, height);
        printf("x=%d,y=%d,wi=%d,he=%d\n", roi.x, roi.y, roi.width, roi.height);
            resize_input_image.copyTo(white_background(roi));


 
    }

    return white_background;
}

//添加背景
cv::Mat addBackground(const cv::Mat& input_image, const cv::Vec3b& bgr, const std::string& mode ) {
	if (input_image.empty() || input_image.channels() != 4) {
		std::cerr << "Input image is empty or does not have an alpha channel." << std::endl;
		return cv::Mat();
	}

	int height = input_image.rows;
	int width = input_image.cols;
	std::vector<cv::Mat> channels(4);
	cv::split(input_image, channels);

	cv::Mat b = channels[0];
	cv::Mat g = channels[1];
	cv::Mat r = channels[2];
	cv::Mat a = channels[3];
	cv::Mat a_cal;
	a.convertTo(a_cal, CV_32F, 1.0 / 255);

	cv::Mat b2, g2, r2;
	if (mode == "pure_color") {
		b2 = cv::Mat(height, width, CV_8UC1, cv::Scalar(bgr[0]));
		g2 = cv::Mat(height, width, CV_8UC1, cv::Scalar(bgr[1]));
		r2 = cv::Mat(height, width, CV_8UC1, cv::Scalar(bgr[2]));
	}
	else {
		// If gradient background is needed, generate_gradient function should be implemented.
	}

	cv::Mat output_b = cv::Mat::zeros(height, width, CV_8UC1);
	cv::Mat output_g = cv::Mat::zeros(height, width, CV_8UC1);
	cv::Mat output_r = cv::Mat::zeros(height, width, CV_8UC1);

	for (int y = 0; y < height; ++y) {
		for (int x = 0; x < width; ++x) {
			output_b.at<uchar>(y, x) = (b.at<uchar>(y, x) - b2.at<uchar>(y, x)) * a_cal.at<float>(y, x) + b2.at<uchar>(y, x);
			output_g.at<uchar>(y, x) = (g.at<uchar>(y, x) - g2.at<uchar>(y, x)) * a_cal.at<float>(y, x) + g2.at<uchar>(y, x);
			output_r.at<uchar>(y, x) = (r.at<uchar>(y, x) - r2.at<uchar>(y, x)) * a_cal.at<float>(y, x) + r2.at<uchar>(y, x);
		}
	}

	std::vector<cv::Mat> mergedChannels = { output_b, output_g, output_r, a };
	cv::Mat output_image;
	cv::merge(mergedChannels, output_image);

	return output_image;
}
