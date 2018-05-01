#ifndef IMAGE_SHADER_H_
#define IMAGE_SHADER_H_

#include "shader.hpp"
#include <storage/objloader.hpp>
#include <render/gltexture.hpp>
using namespace nanogui;

class ImageShader : public NanoShader
{
public:
    ImageShader();
    void Init();
    void SetTexture(GLTexture& texture) {
        mImageID = texture.texture();
    }
    void Draw();
    virtual void bind() {
        mShader.bind();
    }

    Vector2f imagePosition, scaleFactor;
    GLuint mImageID;
    
    static nanogui::GLShader mShader;

    char const *const defaultImageViewVertexShader =
        R"(#version 330
        uniform vec2 scaleFactor;
        uniform vec2 position;
        in vec2 vertex;
        out vec2 uv;
        void main() {
            uv = vertex;
            vec2 scaledVertex = (vertex * scaleFactor) + position;
            gl_Position  = vec4(2.0*scaledVertex.x - 1.0,
                                1.0 - 2.0*scaledVertex.y,
                                0.0, 1.0);

        })";

    char const *const defaultImageViewFragmentShader =
        R"(#version 330
        uniform sampler2D image;
        out vec4 color;
        in vec2 uv;
        void main() {
            color = texture(image, uv);
        })";

    MatrixXf positions;
    MatrixXu indices;

};
#endif
