#ifndef MODEL_SHADER_H_
#define MODEL_SHADER_H_

#include "shader.hpp"
#include <io/objloader.hpp>
#include <render/gltexture.hpp>

class ModelShader : public NanoShader
{
public:
    ModelShader();
    void Init(const objl::Mesh& mesh, const std::string& path);
    void SetModelMatrix(Eigen::Matrix4f model);
    void SetIntrinsic(Eigen::Matrix3f intrinsic, float wx, float wy);
    void SetExtrinsic(Eigen::Matrix4f world2cam);
    void Draw();

    GLTexture map_Kd, map_Ka, map_Ks, map_d, map_bump;
    int num_indices;
};
#endif
