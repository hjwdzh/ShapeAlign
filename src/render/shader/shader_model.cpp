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
             "a_model_shader",
             
             "#version 330\n"
             "uniform float fx;\n"
             "uniform float fy;\n"
             "uniform float cx;\n"
             "uniform float cy;\n"
             "uniform mat4 view;\n"
             "uniform mat4 model;\n"
             "uniform vec3 lightdir;\n"
             "in vec3 position;\n"
             "in vec2 texcoord;\n"
             "in vec3 normal;\n"
             "in vec3 color;\n"
             "out vec3 frag_light;\n"
             "out vec4 frag_color;\n"
             "out vec3 frag_pos;\n"
             "out vec3 frag_normal;\n"
             "out vec2 frag_texcoord;\n"
             "void main() {\n"
             "  mat4 modelView = view * model;\n"
             "	frag_texcoord = texcoord;\n"
             "	frag_light = normalize(lightdir);\n"
             "  frag_pos = (modelView * vec4(position, 1.0f)).xyz;\n"
             //"  frag_pos.z += 1.0f;\n"
             "  frag_color = vec4(color, 1.0f);\n"
             "  frag_normal = (modelView * vec4(normal, 0.0f)).xyz;\n"
             "  gl_Position = vec4(frag_pos.x * fx + cx, frag_pos.y * fy + cy, -0.01 + 0.9 * frag_pos.z, frag_pos.z);\n"
             "}",
             
             "#version 330\n"
             "uniform vec3 lightcolor;\n"
             "uniform vec3 Ka;\n"
             "uniform vec3 Kd;\n"
             "uniform vec3 Ks;\n"
             "uniform float Ns;\n"
             "uniform float Ni;\n"
             "uniform float d;\n"
             "uniform int illum;\n"
             "uniform sampler2D map_Ka;\n"
             "uniform sampler2D map_Kd;\n"
             "uniform sampler2D map_Ks;\n"
             "uniform sampler2D map_d;\n"
             "uniform sampler2D map_bump;\n"
             "uniform int render_color;\n"
             "out vec4 color;\n"
             "in vec3 frag_light;\n"
             "in vec4 frag_color;\n"
             "in vec3 frag_pos;\n"
             "in vec3 frag_normal;\n"
             "in vec2 frag_texcoord;\n"
             "void main() {\n"
             "	if (render_color == 1)\n"
             "		color = frag_color;\n"
             "	else {\n"
             "		vec3 c = Ka * lightcolor;\n"
             "		vec3 lightpos = normalize(frag_pos);\n"
             "		vec3 surface_normal = normalize(frag_normal);\n"
             "      float t = -dot(frag_light, surface_normal);\n"
             "		if (t > 0) {\n"
             "			vec3 reflect_dir = normalize(frag_pos);"
             "			c += Kd * lightcolor * (t, -dot(surface_normal, reflect_dir));\n"
             "		}\n"
             "\n"
             "\n"
             "\n"
             "\n"
             "\n"
             "\n"
             "\n"
             "\n"
             "\n"
             "\n"
             "\n"
             "\n"
             "\n"
             "\n"
             "\n"
             "\n"
             "\n"
             "\n"
             "      color = vec4(c, 1.0f);\n"
             "	}"
             "}"
             );
    MatrixXf positions(3, mesh.Vertices.size());
    MatrixXf colors(3, mesh.Vertices.size());
    MatrixXf normals(3, mesh.Vertices.size());
    MatrixXf texcoords(2, mesh.Vertices.size());
    for (int i = 0; i < mesh.Vertices.size(); ++i) {
    	positions.col(i) << mesh.Vertices[i].Position.X, mesh.Vertices[i].Position.Y, mesh.Vertices[i].Position.Z;
    	colors.col(i) << mesh.Vertices[i].Color.X, mesh.Vertices[i].Color.Y, mesh.Vertices[i].Color.Z;
        normals.col(i) << mesh.Vertices[i].Normal.X, mesh.Vertices[i].Normal.Y, mesh.Vertices[i].Normal.Z;
        texcoords.col(i) << mesh.Vertices[i].TextureCoordinate.X, mesh.Vertices[i].TextureCoordinate.Y;
    }
    
    MatrixXu indices(3, mesh.Indices.size() / 3);
    for (int j = 0; j < mesh.Indices.size(); j += 3)
	{
		indices.col(j / 3) << mesh.Indices[j], mesh.Indices[j + 1], mesh.Indices[j + 2];
	}
    
    num_indices = mesh.Indices.size();
    
    mShader.bind();

    if (mesh.MeshMaterial.name.empty()) {
        mShader.setUniform("render_color", 1);
    } else {
        mShader.setUniform("render_color", 0);
        /*
        file << "Material: " << curMesh.MeshMaterial.name << "\n";
        file << "Ambient Color: " << curMesh.MeshMaterial.Ka.X << ", " << curMesh.MeshMaterial.Ka.Y << ", " << curMesh.MeshMaterial.Ka.Z << "\n";
        file << "Diffuse Color: " << curMesh.MeshMaterial.Kd.X << ", " << curMesh.MeshMaterial.Kd.Y << ", " << curMesh.MeshMaterial.Kd.Z << "\n";
        file << "Specular Color: " << curMesh.MeshMaterial.Ks.X << ", " << curMesh.MeshMaterial.Ks.Y << ", " << curMesh.MeshMaterial.Ks.Z << "\n";
        file << "Specular Exponent: " << curMesh.MeshMaterial.Ns << "\n";
        file << "Optical Density: " << curMesh.MeshMaterial.Ni << "\n";
        file << "Dissolve: " << curMesh.MeshMaterial.d << "\n";
        file << "Illumination: " << curMesh.MeshMaterial.illum << "\n";
        file << "Ambient Texture Map: " << curMesh.MeshMaterial.map_Ka << "\n";
        file << "Diffuse Texture Map: " << curMesh.MeshMaterial.map_Kd << "\n";
        file << "Specular Texture Map: " << curMesh.MeshMaterial.map_Ks << "\n";
        file << "Alpha Texture Map: " << curMesh.MeshMaterial.map_d << "\n";
        file << "Bump Map: " << curMesh.MeshMaterial.map_bump << "\n";
        */
        mShader.setUniform("Ka", Eigen::Vector3f(mesh.MeshMaterial.Ka.X, mesh.MeshMaterial.Ka.Y, mesh.MeshMaterial.Ka.Z));
        mShader.setUniform("Kd", Eigen::Vector3f(mesh.MeshMaterial.Kd.X, mesh.MeshMaterial.Kd.Y, mesh.MeshMaterial.Kd.Z));
        mShader.setUniform("Ks", Eigen::Vector3f(mesh.MeshMaterial.Ks.X, mesh.MeshMaterial.Ks.Y, mesh.MeshMaterial.Ks.Z));
        mShader.setUniform("Ns", mesh.MeshMaterial.Ns);
        mShader.setUniform("Ki", mesh.MeshMaterial.Ni);
        mShader.setUniform("d",  mesh.MeshMaterial.d);
        mShader.setUniform("illum", mesh.MeshMaterial.illum);
        mShader.setUniform("lightdir",  Eigen::Vector3f(0.0f, 0.0f, 1.0f));
        mShader.setUniform("lightcolor",  Eigen::Vector3f(1.0f, 1.0f, 1.0f));
    }

    mShader.uploadIndices(indices);
    
    mShader.uploadAttrib("position", positions);
    mShader.uploadAttrib("color", colors);	
    mShader.uploadAttrib("normal", normals);
    mShader.uploadAttrib("texcoords", texcoords);
}

void ModelShader::Draw()
{
    mShader.drawIndexed(GL_TRIANGLES, 0, num_indices);
}

void ModelShader::SetIntrinsic(Eigen::Matrix3f intrinsic, float wx, float wy)
{
    mShader.bind();
    mShader.setUniform("fx", intrinsic(0, 0) / wx * 2.0);
    mShader.setUniform("fy", intrinsic(1, 1) / wy * 2.0);
    mShader.setUniform("cx", intrinsic(0, 2) / wx * 2.0 - 1.0);
    mShader.setUniform("cy", intrinsic(1, 2) / wy * 2.0 - 1.0);
}

void ModelShader::SetExtrinsic(Eigen::Matrix4f world2cam) {
    mShader.bind();
    mShader.setUniform("view", world2cam);
}
