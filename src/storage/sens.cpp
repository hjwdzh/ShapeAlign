#include "sens.h"
#include <fstream>
#include <iostream>

SensData::SensData(const std::string& filename)
{
	FILE* fp = fopen(filename.c_str(), "rb");
    fread(&depth_width, sizeof(int), 1, fp);
    fread(&depth_height, sizeof(int), 1, fp);
    fread(&color_width, sizeof(int), 1, fp);
    fread(&color_height, sizeof(int), 1, fp);
    intrinsic = cv::Mat::zeros(4, 4, CV_32F);
    fread(intrinsic.data, sizeof(float), 16, fp);
    intrinsic.at<float>(2, 2) = 1;
    intrinsic.at<float>(2, 3) = 0;
    intrinsic.at<float>(3, 0) = 0;
    intrinsic.at<float>(3, 1) = 0;
    intrinsic.at<float>(3, 2) = 0;
    intrinsic.at<float>(3, 3) = 1;
    extrinsic = cv::Mat::zeros(4, 4, CV_32F);
    fread(extrinsic.data, sizeof(float), 16, fp);
    fread(&frames, sizeof(int), 1, fp);
    colors.resize(frames);
    depths.resize(frames);
    cam2world.resize(frames);
    std::cout << depth_width << " " << depth_height << " " << color_width << " " << color_height << " "
    << frames << "\n";
    std::cout << "Loading frames...\n";
    for (int i = 0; i < frames; ++i) {
        std::cout << "Loading " << i + 1 << " of " << frames << "\n";
        int len;
        fread(&len, sizeof(int), 1, fp);
        depths[i] = cv::Mat(depth_height, depth_width, CV_32F);
        fread(depths[i].data, sizeof(unsigned char), len, fp);
        fread(&len, sizeof(int), 1, fp);
        cv::Mat raw = cv::Mat(1, len, CV_8U);
        fread(raw.data, sizeof(unsigned char), len, fp);
        colors[i] = imdecode(raw, CV_LOAD_IMAGE_COLOR);
        cam2world[i] = cv::Mat(4, 4, CV_32F);
        fread(cam2world[i].data, sizeof(float), 16, fp);
    }
    fclose(fp);
}

void SensData::GenImages(const std::string& output_path)
{
    char buffer[1024];
    for (int i = 0; i < depths.size(); ++i) {
        printf("Export frame %d of %d\n", i + 1, (int)depths.size());
        cv::Mat depth;
        depths[i].convertTo(depth, CV_16UC1, 1);
        sprintf(buffer, "%s/%04d_depth.png", output_path.c_str(), i + 1);
        cv::imwrite(buffer, depth);
        cv::Mat color;
        cv::resize(colors[i], color, cv::Size(depth.cols, depth.rows));
        cv::cvtColor(color, color, CV_RGB2BGR);
        sprintf(buffer, "%s/%04d_color.png", output_path.c_str(), i + 1);
        cv::imwrite(buffer, color);
    }
}

void SensData::GenMeta(const std::string& output_path)
{
    std::ofstream os(output_path + "/meta.txt");
    os << depth_width << " " << depth_height << " " << color_width << " " << color_height << "\n";
    os << "\n";
    for (int i = 0; i < 3; ++i) {
        for (int j = 0; j < 3; ++j) {
            os << intrinsic.at<float>(i, j) << " ";
        }
        os << "\n";
    }
    os << "\n";
    os << cam2world.size() << "\n";
    os << "\n";
    for (int i = 0; i < cam2world.size(); ++i) {
        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                os << cam2world[i].at<float>(j, k) << " ";
            }
            os << "\n";
        }
        os << "\n";
    }
}

void SensData::GenPointcloud(const std::string& output_path)
{
    char buffer[1024];
    for (int i = 0; i < depths.size(); ++i) {
        printf("Export pointcloud %d of %d\n", i + 1, (int)depths.size());
        cv::Mat d = depths[i];
        cv::Mat c;
        cv::resize(colors[i], c, cv::Size(d.cols, d.rows));
        sprintf(buffer, "%s/%04d_pts.obj", output_path.c_str(), i + 1);

        for (int j = 0; j < 4; ++j) {
            for (int k = 0; k < 4; ++k) {
                std::cout << cam2world[i].at<float>(j, k) << " ";
            }
            std::cout << "\n";
        }
        std::cout << "\n";
        std::ofstream os(buffer);
        for (int j = 0; j < d.rows; j += 4) {
            for (int k = 0; k < d.cols; k += 4) {
                float depth = d.at<float>(j, k);
                if (!(depth < 10000 && depth > 100))
                    continue;
                cv::Mat pt(4, 1, CV_32F);
                pt.at<float>(2, 0) = depth * 1e-3f;
                pt.at<float>(0, 0) = (k - intrinsic.at<float>(0, 2)) / intrinsic.at<float>(0, 0) * pt.at<float>(2, 0);
                pt.at<float>(1, 0) = (j - intrinsic.at<float>(1, 2)) / intrinsic.at<float>(1, 1) * pt.at<float>(2, 0);
                pt.at<float>(3, 0) = 1;
                pt = cam2world[i] * pt;
                cv::Vec3b p = c.at<cv::Vec3b>(j, k);
                os << "v " << pt.at<float>(0, 0) << " " << pt.at<float>(1, 0) << " " << pt.at<float>(2, 0)
                    << " " << (int)p.val[0] << " " << (int)p.val[1] << " " << (int)p.val[2] << "\n";
            }
        }
        os.close();
    }
}
