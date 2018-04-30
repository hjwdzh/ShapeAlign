#include "canvas_model.hpp"

ModelCanvas::ModelCanvas(Widget *parent)
: nanogui::GLCanvas(parent), mRotation(nanogui::Vector3f(0.25f, 0.5f, 0.33f))
{
}

void ModelCanvas::Init(const std::string& filename) {
    bool loadout = obj_model.LoadFile(filename);
    if (!loadout) {
        std::cerr << "Unable to load " << filename << "\n";
        return;
    }
    mShaders.resize(obj_model.LoadedMeshes.size());
    for (int i = 0; i < mShaders.size(); i++) {
        mShaders[i].Init(obj_model.LoadedMeshes[i]);
    }
}

void ModelCanvas::setRotation(nanogui::Vector3f vRotation) {
    mRotation = vRotation;
}


void ModelCanvas::drawGL() {
    using namespace nanogui;
    
    glEnable(GL_DEPTH_TEST);
    for (auto& mShader : mShaders) {
        mShader.bind();
        
        Matrix4f view = Eigen::Matrix4f::Identity();
        Matrix4f model, proj;
        model.setIdentity();
        proj.setIdentity();
        float fTime = (float)glfwGetTime();
        
        model.topLeftCorner<3,3>() = Eigen::Matrix3f(Eigen::AngleAxisf(mRotation[0]*fTime, Vector3f::UnitX()) *
                                                   Eigen::AngleAxisf(mRotation[1]*fTime,  Vector3f::UnitY()) *
                                                   Eigen::AngleAxisf(mRotation[2]*fTime, Vector3f::UnitZ())) * 0.25f;
        
        Eigen::Matrix3f intrinsic = Eigen::Matrix3f::Zero();
        intrinsic(0, 0) = 575;
        intrinsic(1, 1) = 575;
        intrinsic(0, 2) = 200;
        intrinsic(1, 2) = 200;
        view(2, 3) = 1.0f + fTime * 0.1f;
        mShader.mShader.setUniform("model", model);
        mShader.SetExtrinsic(view);
        mShader.SetIntrinsic(intrinsic, 400, 400);
        /* Draw 12 triangles starting at index 0 */
        for (int i = 0; i < mShaders.size(); ++i) {
            mShaders[i].Draw();
        }
    }
    glDisable(GL_DEPTH_TEST);
}
