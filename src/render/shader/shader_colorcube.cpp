#include "shader_colorcube.hpp"
using namespace nanogui;

ColorcubeShader::ColorcubeShader()
{
    mShader.init(
                 /* An identifying name */
                 "a_simple_shader",
                 
                 /* Vertex shader */
                 "#version 330\n"
                 "uniform mat4 modelViewProj;\n"
                 "in vec3 position;\n"
                 "in vec3 color;\n"
                 "out vec4 frag_color;\n"
                 "void main() {\n"
                 "    frag_color = 3.0 * modelViewProj * vec4(color, 1.0);\n"
                 "    gl_Position = modelViewProj * vec4(position, 1.0);\n"
                 "}",
                 
                 /* Fragment shader */
                 "#version 330\n"
                 "out vec4 color;\n"
                 "in vec4 frag_color;\n"
                 "void main() {\n"
                 "    color = frag_color;\n"
                 "}"
                 );
    
    MatrixXu indices(3, 12); /* Draw a cube */
    indices.col( 0) << 0, 1, 3;
    indices.col( 1) << 3, 2, 1;
    indices.col( 2) << 3, 2, 6;
    indices.col( 3) << 6, 7, 3;
    indices.col( 4) << 7, 6, 5;
    indices.col( 5) << 5, 4, 7;
    indices.col( 6) << 4, 5, 1;
    indices.col( 7) << 1, 0, 4;
    indices.col( 8) << 4, 0, 3;
    indices.col( 9) << 3, 7, 4;
    indices.col(10) << 5, 6, 2;
    indices.col(11) << 2, 1, 5;
    
    MatrixXf positions(3, 8);
    positions.col(0) << -1,  1,  1;
    positions.col(1) << -1,  1, -1;
    positions.col(2) <<  1,  1, -1;
    positions.col(3) <<  1,  1,  1;
    positions.col(4) << -1, -1,  1;
    positions.col(5) << -1, -1, -1;
    positions.col(6) <<  1, -1, -1;
    positions.col(7) <<  1, -1,  1;
    
    MatrixXf colors(3, 12);
    colors.col( 0) << 1, 0, 0;
    colors.col( 1) << 0, 1, 0;
    colors.col( 2) << 1, 1, 0;
    colors.col( 3) << 0, 0, 1;
    colors.col( 4) << 1, 0, 1;
    colors.col( 5) << 0, 1, 1;
    colors.col( 6) << 1, 1, 1;
    colors.col( 7) << 0.5, 0.5, 0.5;
    colors.col( 8) << 1, 0, 0.5;
    colors.col( 9) << 1, 0.5, 0;
    colors.col(10) << 0.5, 1, 0;
    colors.col(11) << 0.5, 1, 0.5;
    
    mShader.bind();
    mShader.uploadIndices(indices);
    
    mShader.uploadAttrib("position", positions);
    mShader.uploadAttrib("color", colors);
}

void ColorcubeShader::Draw()
{
    mShader.drawIndexed(GL_TRIANGLES, 0, 12);
}
