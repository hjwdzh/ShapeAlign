#include "shader_image.hpp"
#include <storage/persistent.hpp>
#include <application/app_shapealign.hpp>
using namespace nanogui;
extern ShapeAlignApplication* g_app;

nanogui::GLShader ImageShader::mShader;
ImageShader::ImageShader()
{
    imagePosition = Vector2f::Zero();
    scaleFactor = Vector2f(1, 1);
}

void ImageShader::Init()
{
    mShader.init("ImageViewShader", defaultImageViewVertexShader,
                 defaultImageViewFragmentShader);

    MatrixXu indices(3, 2);
    indices.col(0) << 0, 1, 2;
    indices.col(1) << 2, 3, 1;

    MatrixXf vertices(2, 4);
    vertices.col(0) << 0, 0;
    vertices.col(1) << 1, 0;
    vertices.col(2) << 0, 1;
    vertices.col(3) << 1, 1;

    mShader.bind();
    mShader.uploadIndices(indices);
    mShader.uploadAttrib("vertex", vertices);
}

void ImageShader::Draw()
{
    if (sens_data.selected >= sens_data.frames)
        return;
    mShader.bind();
    SetTexture(g_app->textures[sens_data.selected]);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, mImageID);
    mShader.setUniform("image", 0);
    mShader.setUniform("scaleFactor", scaleFactor);
    mShader.setUniform("position", imagePosition);
    mShader.drawIndexed(GL_TRIANGLES, 0, 2);
}
