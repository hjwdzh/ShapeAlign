#ifndef IMAGE_CANVAS_H_
#define IMAGE_CANVAS_H_

#include <render/shader/shader_image.hpp>
#include <render/shader/shader_model.hpp>
#include <nanogui_header.hpp>

class ImageCanvas : public nanogui::GLCanvas {
public:
    ImageCanvas(Widget *parent)
    : nanogui::GLCanvas(parent)
    {
    	imgShader.Init();
    }

    void AddElement(const std::string& filename);
    ImageShader imgShader;
    void drawGL();

    std::vector<ModelShader*> mShaders;
    std::set<std::string> mModelName;
};

#endif
