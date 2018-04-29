#include "canvas_colorcube.hpp"

ColorcubeCanvas::ColorcubeCanvas(Widget *parent)
: nanogui::GLCanvas(parent), mRotation(nanogui::Vector3f(0.25f, 0.5f, 0.33f))
{
}

void ColorcubeCanvas::setRotation(nanogui::Vector3f vRotation) {
    mRotation = vRotation;
}


void ColorcubeCanvas::drawGL() {
    using namespace nanogui;
    
    mShader.bind();
    
    Matrix4f mvp;
    mvp.setIdentity();
    float fTime = (float)glfwGetTime();
    mvp.topLeftCorner<3,3>() = Eigen::Matrix3f(Eigen::AngleAxisf(mRotation[0]*fTime, Vector3f::UnitX()) *
                                               Eigen::AngleAxisf(mRotation[1]*fTime,  Vector3f::UnitY()) *
                                               Eigen::AngleAxisf(mRotation[2]*fTime, Vector3f::UnitZ())) * 0.25f;
    
    mShader.mShader.setUniform("modelViewProj", mvp);
    
    glEnable(GL_DEPTH_TEST);
    /* Draw 12 triangles starting at index 0 */
    mShader.Draw();
    glDisable(GL_DEPTH_TEST);
}
