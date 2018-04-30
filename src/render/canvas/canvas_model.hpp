#ifndef MODEL_CANVAS_H_
#define MODEL_CANVAS_H_

#include <render/shader/shader_model.hpp>
#include <render/shader/shader_colorcube.hpp>

class ModelCanvas : public nanogui::GLCanvas {
public:
    ModelCanvas(Widget *parent);

    void Init(const std::string& filename);

    virtual void drawGL() override;
    virtual bool mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers) override;
    virtual bool mouseMotionEvent(const Eigen::Vector2i &p, const Eigen::Vector2i &rel, int button, int modifiers) override;
    virtual bool scrollEvent(const Eigen::Vector2i &p, const Eigen::Vector2f &rel) override;
    
    Eigen::Matrix4f world2cam;
    Eigen::Matrix4f model;
private:
    Eigen::Vector2i mouse;
    float trans_scale;
    int mouse_state;
	std::vector<ModelShader> mShaders;
    objl::Loader obj_model;
};

#endif
