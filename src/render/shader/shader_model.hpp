#ifndef MODEL_SHADER_H_
#define MODEL_SHADER_H_

#include "shader.hpp"
#include <io/objloader.hpp>

class ModelShader : public NanoShader
{
public:
    ModelShader();
    void Init(const objl::Mesh& mesh);
    
    void Draw();

    int num_indices;
};
#endif
