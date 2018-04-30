#ifndef MODEL_SHADER_H_
#define MODEL_SHADER_H_

#include "shader.hpp"
#include <storage/objloader.hpp>
#include <render/gltexture.hpp>
using namespace nanogui;
class ModelShader : public NanoShader
{
public:
    ModelShader();
    void Init(const objl::Mesh& mesh, const std::string& path);
    void SetModelMatrix(Eigen::Matrix4f model);
    void SetIntrinsic(Eigen::Matrix3f intrinsic, float wx, float wy);
    void SetExtrinsic(Eigen::Matrix4f world2cam);
    void Draw();
    virtual void bind() {
        mShader.bind();
    }
    
    static nanogui::GLShader mShader;

    GLTexture map_Kd, map_Ka, map_Ks, map_d, map_bump;
    int num_indices;
    static int shared_shader_count;
    std::string filename;
    
    MatrixXf positions;
    MatrixXf colors;
    MatrixXf normals;
    MatrixXf texcoords;
    MatrixXu indices;

};
#endif
