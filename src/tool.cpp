#include "main.h"





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
