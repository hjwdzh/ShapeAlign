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
        
        Matrix4f mvp;
        mvp.setIdentity();
        float fTime = (float)glfwGetTime();
        mvp.topLeftCorner<3,3>() = Eigen::Matrix3f(Eigen::AngleAxisf(mRotation[0]*fTime, Vector3f::UnitX()) *
                                                   Eigen::AngleAxisf(mRotation[1]*fTime,  Vector3f::UnitY()) *
                                                   Eigen::AngleAxisf(mRotation[2]*fTime, Vector3f::UnitZ())) * 0.25f;
    
        mShader.mShader.setUniform("modelViewProj", mvp);
        /* Draw 12 triangles starting at index 0 */
        for (int i = 0; i < mShaders.size(); ++i) {
            mShaders[i].Draw();
        }
    }
    glDisable(GL_DEPTH_TEST);
}
