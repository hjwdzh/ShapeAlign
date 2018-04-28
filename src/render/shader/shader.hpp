#ifndef SHADER_H_
#define SHADER_H_

#include <nanogui_header.hpp>

class NanoShader
{
public:
    NanoShader() {}
    virtual void Draw() = 0;
    ~NanoShader() {
        mShader.free();
    }
    void bind() {
        mShader.bind();
    }
    nanogui::GLShader mShader;
};

#endif
