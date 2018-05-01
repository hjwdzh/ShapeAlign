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
