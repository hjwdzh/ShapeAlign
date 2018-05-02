#include "canvas_image.hpp"

#include <application/app_shapealign.hpp>
#include <storage/persistent.hpp>

extern ShapeAlignApplication* g_app;

void ImageCanvas::AddElement(const std::string& filename) {
    objl::Loader* obj_model = OBJData::AddElement(filename);
    for (int i = 0; i < obj_model->LoadedMeshes.size(); i++) {
        mShaders.push_back(new ModelShader());
        mShaders.back()->filename = filename;
        mShaders.back()->Init(obj_model->LoadedMeshes[i], GetDirectory(filename));
    }
}


void ImageCanvas::drawGL()
{
    if (sens_data.selected >= sens_data.frames)
        return;

    imgShader.SetTexture(g_app->textures[sens_data.frames]);
    imgShader.Draw();

    if (g_app->view_model != 0) {
    
    using namespace nanogui;
    
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    
    int top = 0;
    for (int i = 0; i < mShaders.size(); ++i) {
        if (OBJData::GetElement(mShaders[i]->filename)) {
            mShaders[top++] = mShaders[i];
        }
    }
    mShaders.resize(top);
    for (auto& mShader : mShaders) {
        mShader->bind();
        Eigen::Matrix3f intrinsic = Eigen::Matrix3f::Identity();
        
        intrinsic(0, 0) = sens_data.intrinsic.at<float>(0, 0);
        intrinsic(1, 1) = sens_data.intrinsic.at<float>(1, 1);
        intrinsic(0, 2) = sens_data.intrinsic.at<float>(0, 2);
        intrinsic(1, 2) = sens_data.intrinsic.at<float>(1, 2);
        
        cv::Mat m = sens_data.cam2world[sens_data.selected].inv();
        Eigen::Matrix4f extrinsic;
        extrinsic.setIdentity();
        for (int i = 0; i < 4; ++i) {
            for (int j = 0; j < 4; ++j) {
                extrinsic(i, j) = m.at<float>(i, j);
            }
        }
        mShader->SetExtrinsic(extrinsic);
        mShader->SetIntrinsic(intrinsic, width(), height());
        mShader->Draw();
    }

    glDisable(GL_TEXTURE_2D);
    glDisable(GL_DEPTH_TEST);
        
    }
    
    if (g_app->keypoint_mode == 0)
        return;
    
    Eigen::MatrixXf positions(3, keypoints.size() / 2), colors(3, keypoints.size() / 2);
    for (int i = 0; i < keypoints.size(); i += 2) {
        if (keyframes[i / 2] == sens_data.selected)
            positions.col(i / 2) << keypoints[i], -keypoints[i + 1], 0;
        else
            positions.col(i / 2) << -2, -2, 0;
        colors.col(i / 2) = g_app->color_pts[i / 2];
    }
    glPointSize(10.0);
    ModelShader::mShader.bind();
    ModelShader::mShader.uploadAttrib("position", positions);
    ModelShader::mShader.uploadAttrib("color", colors);
    ModelShader::mShader.setUniform("render_2d", 1);
    ModelShader::mShader.setUniform("render_color", 1);
    ModelShader::mShader.drawArray(GL_POINTS, 0, positions.size() / 3);
}

bool ImageCanvas::mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers)
{
    if (sens_data.selected >= sens_data.frames)
        return true;
    if (g_app->keypoint_mode == 0)
        return true;
    Eigen::Vector2i pt = p - position();
    if (button == 0 && down) {
        keypoints.push_back(pt[0] / (float)width() * 2.0f - 1.0f);
        keypoints.push_back(pt[1] / (float)height() * 2.0f - 1.0f);
        keyframes.push_back(sens_data.selected);
    }
    if (button == 1 && down) {
        if (keypoints.size() > 0) {
        keypoints.pop_back();
        keypoints.pop_back();
        keyframes.pop_back();
        }
    }
    return true;
}

