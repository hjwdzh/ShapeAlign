#include "shader_model.hpp"
using namespace nanogui;
#include "shader_colorcube.hpp"
using namespace nanogui;

ModelShader::ModelShader()
: num_indices(0)
{
}

void ModelShader::Init(const objl::Mesh& mesh)
{
    mShader.init(
             "a_simple_shader",
             
             "#version 330\n"
             "uniform mat4 modelViewProj;\n"
             "in vec3 position;\n"
             "in vec3 color;\n"
             "out vec4 frag_color;\n"
             "void main() {\n"
             "    frag_color = vec4(color, 1.0);\n"
             "    gl_Position = modelViewProj * vec4(position, 1.0);\n"
             "}",
             
             "#version 330\n"
             "out vec4 color;\n"
             "in vec4 frag_color;\n"
             "void main() {\n"
             "    color = frag_color;\n"
             "}"
             );
    MatrixXf positions(3, mesh.Vertices.size());
    MatrixXf colors(3, mesh.Vertices.size());
    for (int i = 0; i < mesh.Vertices.size(); ++i) {
    	positions.col(i) << mesh.Vertices[i].Position.X, mesh.Vertices[i].Position.Y, mesh.Vertices[i].Position.Z;
    	colors.col(i) << mesh.Vertices[i].Color.X, mesh.Vertices[i].Color.Y, mesh.Vertices[i].Color.Z;
    }
    
    MatrixXu indices(3, mesh.Indices.size() / 3);
    for (int j = 0; j < mesh.Indices.size(); j += 3)
	{
		indices.col(j / 3) << mesh.Indices[j], mesh.Indices[j + 1], mesh.Indices[j + 2];
	}

    
    num_indices = mesh.Indices.size();
    
    mShader.bind();
    mShader.uploadIndices(indices);
    
    mShader.uploadAttrib("position", positions);
    mShader.uploadAttrib("color", colors);	
}

void ModelShader::Draw()
{
    mShader.drawIndexed(GL_TRIANGLES, 0, num_indices);
}

