#ifndef LOADER_H_
#define LOADER_H_

#include <opencv2/opencv.hpp>
#include <vector>

class SensData
{
public:
	SensData()
	{
        frames = 0;
        selected = 0;
    }
	SensData(const std::string& filename);
    
	void GenImages(const std::string& output_path);
	void GenMeta(const std::string& output_path);
	void GenPointcloud(const std::string& output_path);

	// data
	int depth_width, depth_height, color_width, color_height;
	cv::Mat intrinsic;
	cv::Mat extrinsic;
	int frames;
    int selected;
	std::vector<cv::Mat> depths;
	std::vector<cv::Mat> colors;
	std::vector<cv::Mat> cam2world;
};

#endif
