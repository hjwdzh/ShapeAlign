#include "pose.hpp"
#include "persistent.hpp"
#include <ceres/ceres.h>
#include <ceres/rotation.h>

struct ReprojectionError {
    ReprojectionError(double observed_x, double observed_y, double px, double py, double pz, cv::Mat cam2world, cv::Mat intrinsic)
    : observed_x(observed_x), observed_y(observed_y) {
        cv::Mat proj_cv(intrinsic * cam2world.inv());
        p[0] = px;
        p[1] = py;
        p[2] = pz;
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                proj[i * 4 + j] = proj_cv.at<float>(i, j);
            }
        }
    }
    double observed_x, observed_y;
    double p[3];
    double proj[16];
    
    template <typename T>
    bool operator()(const T* const model,
                    T* residuals) const {
        // camera[0,1,2] are the angle-axis rotation.
        T pt[3];
        T world_pt[3];
        pt[0] = (T)p[0];
        pt[1] = (T)p[1];
        pt[2] = (T)p[2];
        ceres::AngleAxisRotatePoint(model, pt, world_pt);
        // camera[3,4,5] are the translation.
        world_pt[0] += model[3]; world_pt[1] += model[4]; world_pt[2] += model[5];
        T cam_pt[3];
        for (int i = 0; i < 3; ++i) {
            cam_pt[i] = (T)proj[i * 4 + 3];
            for (int j = 0; j < 3; ++j) {
                cam_pt[i] += proj[i * 4 + j] * world_pt[j];
            }
        }
        // The error is the difference between the predicted and observed position.
        residuals[0] = cam_pt[0] / cam_pt[2] - T(observed_x);
        residuals[1] = cam_pt[1] / cam_pt[2] - T(observed_y);
        return true;
    }
    
    // Factory to hide the construction of the CostFunction object from
    // the client code.
    static ceres::CostFunction* Create(double observed_x, double observed_y, double px, double py, double pz, cv::Mat cam2world, cv::Mat intrinsic) {
        return (new ceres::AutoDiffCostFunction<ReprojectionError, 2, 6>(new ReprojectionError(observed_x, observed_y,
                                                                                               px, py, pz, cam2world, intrinsic)));
    }
};

Eigen::Matrix4f EstimateModel(std::vector<float>& pts, std::vector<float> kpts, std::vector<int>& frames)
{
    ceres::Problem problem;
    double model_transform[6] = {0};
    for (int i = 0, j = 0; i < pts.size(); i += 3, j += 2) {
        double px = pts[i], py = pts[i + 1], pz = pts[i + 2];
        cv::Mat intrinsic = sens_data.intrinsic;
        double observed_x = (kpts[j] + 1) * 0.5 * sens_data.depth_width;
        double observed_y = (kpts[j + 1] + 1) * 0.5 * sens_data.depth_height;
        ceres::CostFunction* cost_function =
        ReprojectionError::Create(observed_x, observed_y, px, py, pz, sens_data.cam2world[frames[i / 3]], intrinsic);
        auto error = ReprojectionError(observed_x, observed_y,
                                       px, py, pz, sens_data.cam2world[frames[i / 3]], intrinsic);
        double residuals[2];
        error.operator()(model_transform, residuals);
        problem.AddResidualBlock(cost_function,
                                 NULL /* squared loss */,
                                 model_transform);
    }
    ceres::Solver::Options options;
    options.linear_solver_type = ceres::DENSE_SCHUR;
    options.minimizer_progress_to_stdout = true;
    ceres::Solver::Summary summary;
    ceres::Solve(options, &problem, &summary);

    double R[9];
    ceres::AngleAxisToRotationMatrix(model_transform, R);
    Eigen::Matrix4f tr;
    tr.setIdentity();
    for (int i = 0; i < 3; ++i) {
        tr(i, 3) = model_transform[i + 3];
        for (int j = 0; j < 3; ++j) {
            tr(i, j) = R[j * 3 + i];
        }
    }
    for (auto& obj : OBJData::objdata) {
        obj.second.model = tr;
    }
}
