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
    
    virtual bool mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers) override;

    void AddElement(const std::string& filename);
    ImageShader imgShader;
    void drawGL() override;

    std::vector<ModelShader*> mShaders;
    std::set<std::string> mModelName;
    std::vector<int> keyframes;
    std::vector<float> keypoints;
};

#endif
