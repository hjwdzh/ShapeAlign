#ifndef POSE_H_
#define POSE_H_

#include <Eigen/Core>
#include <Eigen/Dense>
#include <vector>

void EstimateModel(std::vector<float>& pts, std::vector<float> kpts, std::vector<int>& frames, std::vector<std::string>& filenames);

#endif
