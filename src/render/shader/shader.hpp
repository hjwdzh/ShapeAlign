#ifndef SHADER_H_
#define SHADER_H_

#include <nanogui_header.hpp>

class NanoShader
{
public:
    NanoShader() {}
    virtual void Draw() = 0;
    ~NanoShader() {
    }
    virtual void bind() = 0;
};

#endif
