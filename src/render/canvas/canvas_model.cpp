#include "canvas_model.hpp"
#include <storage/persistent.hpp>
#include <application/app_shapealign.hpp>

extern ShapeAlignApplication* g_app;
ModelCanvas::ModelCanvas(Widget *parent)
: nanogui::GLCanvas(parent)
{
    world2cam = Eigen::Matrix4f::Identity();
    world2cam(2, 3) = 10.0;
    mouse_state = -1;
    trans_scale = 4.0;
}

void ModelCanvas::AddElement(const std::string& filename) {
    objl::Loader* obj_model = OBJData::AddElement(filename);
    mModelName.insert(filename);
    for (int i = 0; i < obj_model->LoadedMeshes.size(); i++) {
        mShaders.push_back(new ModelShader());
        mShaders.back()->filename = filename;
        mShaders.back()->Init(obj_model->LoadedMeshes[i], GetDirectory(filename));
    }
}

void ModelCanvas::drawGL() {
    using namespace nanogui;
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    
    int top = 0;
    for (int i = 0; i < mShaders.size(); ++i) {
        if (OBJData::GetElement(mShaders[i]->filename)) {
            mShaders[top++] = mShaders[i];
        } else {
            //delete mShaders[i];
        }
    }
    mShaders.resize(top);
    for (auto& mShader : mShaders) {
        if (g_app->view_pt == 0 && strcmp(mShader->filename.c_str(), "sens") == 0)
            continue;
        mShader->bind();
        Eigen::Matrix3f intrinsic = Eigen::Matrix3f::Zero();
        intrinsic(0, 0) = height() * 1.1f;
        intrinsic(1, 1) = height() * 1.1f;
        intrinsic(0, 2) = width() * 0.5f - 0.5f;
        intrinsic(1, 2) = height() * 0.5f - 0.5f;

        if (g_app->view_extrinsic && sens_data.selected < sens_data.frames) {
            cv::Mat m = sens_data.cam2world[sens_data.selected].inv();
            Eigen::Matrix4f extrinsic;
            extrinsic.setIdentity();
            for (int i = 0; i < 4; ++i) {
                for (int j = 0; j < 4; ++j) {
                    extrinsic(i, j) = m.at<float>(i, j);
                }
            }
            mShader->SetExtrinsic(extrinsic);
        } else {
            mShader->SetExtrinsic(world2cam);
        }
        mShader->SetIntrinsic(intrinsic, width(), height());
        mShader->Draw();
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
    
    std::vector<Eigen::Vector3f> positions_vec, colors_vec;
    std::vector<int> positions_ind;
    for (int i = 0; i < keypoints.size(); i += 3) {
        if (keyframes[i / 3] == sens_data.selected) {
            positions_vec.push_back(Eigen::Vector3f(keypoints[i], keypoints[i + 1], keypoints[i + 2]));
            colors_vec.push_back(g_app->color_pts[i / 3]);
        }
    }
    Eigen::MatrixXf positions(3, positions_vec.size()), colors(3, colors_vec.size());
    for (int i = 0; i < positions_vec.size(); ++i) {
        positions.col(i) = positions_vec[i];
        colors.col(i) = colors_vec[i];
    }

    ModelShader::mShader.bind();
    if (g_app->view_extrinsic && sens_data.selected < sens_data.frames) {
        cv::Mat m = sens_data.cam2world[sens_data.selected].inv();
        Eigen::Matrix4f extrinsic;
        extrinsic.setIdentity();
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                extrinsic(i, j) = m.at<float>(i, j);
            }
        }
        ModelShader::mShader.setUniform("view", extrinsic);
    } else {
        ModelShader::mShader.setUniform("view", world2cam);
    }
    glPointSize(10.0);
    ModelShader::mShader.uploadAttrib("position", positions);
    ModelShader::mShader.uploadAttrib("color", colors);
//    ModelShader::mShader.setUniform("render_2d", 1);
    ModelShader::mShader.setUniform("render_color", 1);
    ModelShader::mShader.drawArray(GL_POINTS, 0, positions.size() / 3);
}

bool ModelCanvas::mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers)
{
    if (g_app->keypoint_mode && sens_data.selected < sens_data.frames) {
        if (button == 1 && down) {
            keyframes.pop_back();
            keypoints.pop_back();
            keypoints.pop_back();
            keypoints.pop_back();
            return true;
        } else
        if (!(button == 0 && down)) {
            return true;
        }
        Eigen::Vector2i pt = p - position();
        Eigen::Vector4f d(0, 0, 0, 1);
        Eigen::Matrix3f intrinsic = Eigen::Matrix3f::Zero();
        intrinsic(0, 0) = height() * 1.1f;
        intrinsic(1, 1) = height() * 1.1f;
        intrinsic(0, 2) = width() * 0.5f - 0.5f;
        intrinsic(1, 2) = height() * 0.5f - 0.5f;

        Eigen::Vector4f t((pt.x() - intrinsic(0, 2)) / intrinsic(0, 0),
                          (pt.y() - intrinsic(1, 2)) / intrinsic(1, 1),
                          1.0f, 0);
        t.normalize();
        cv::Mat m = sens_data.cam2world[sens_data.selected];
        Eigen::Matrix4f extrinsic;
        extrinsic.setIdentity();
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                extrinsic(i, j) = m.at<float>(i, j);
            }
        }
        d = extrinsic * d;
        t = extrinsic * t;
        
        Vector3f intersect_pt = OBJData::Intersect(d, t);
        if (intersect_pt[0] < 1e20) {
            keypoints.push_back(intersect_pt[0]);
            keypoints.push_back(intersect_pt[1]);
            keypoints.push_back(intersect_pt[2]);
            keyframes.push_back(sens_data.selected);
        }
        return true;
    }
    if (button >= 0 && down) {
        mouse = p - position();
        mouse_state = button;
    } else {
        mouse_state = -1;
    }
    return true;
}

bool ModelCanvas::mouseMotionEvent(const Eigen::Vector2i &p, const Eigen::Vector2i &rel, int button, int modifiers)
{
    Eigen::Vector2i pt = p - position();
    if (mouse_state == 0 || mouse_state == 1) {
        float rotX = -(pt[0] - mouse[0]) / (float)width() * 3.14;
        float rotY = -(pt[1] - mouse[1]) / (float)width() * 3.14;
        Eigen::Matrix4f rotation = Eigen::Matrix4f::Identity();
        Eigen::Matrix4f rotationY = Eigen::Matrix4f::Identity();
        rotation.topLeftCorner<3,3>() = Eigen::Matrix3f(Eigen::AngleAxisf(rotX, Eigen::Vector3f::UnitY()));
        rotationY.topLeftCorner<3,3>() = Eigen::Matrix3f(Eigen::AngleAxisf(rotY, Eigen::Vector3f::UnitX()));
        for (auto& name : mModelName) {
            auto obj = OBJData::GetElement(name);
            if (obj->selected || mouse_state == 0)
                obj->model = rotationY * rotation * obj->model;
        }
        mouse = pt;
    } else
    if (mouse_state == 2) {
        float transX = -(pt[0] - mouse[0]) / (float)width();
        float transY = -(pt[1] - mouse[1]) / (float)width();
        
        for (auto& name : mModelName) {
            auto obj = OBJData::GetElement(name);
            if (obj->selected) {
                obj->model(0, 3) -= transX * trans_scale;
                obj->model(1, 3) += transY * trans_scale;
            }
        }
        mouse = pt;
    }
    return true;
}

bool ModelCanvas::scrollEvent(const Eigen::Vector2i &p, const Eigen::Vector2f &rel)
{
    Eigen::Vector3f trans(world2cam(0, 3), world2cam(1, 3), world2cam(2, 3));
    float s = (float)exp((rel[1]) > 0 ? 0.5 : -0.5);
//    trans_scale *= s;
    trans *= s;
    world2cam.col(3) << trans[0], trans[1], trans[2], 1;

    return true;
}


