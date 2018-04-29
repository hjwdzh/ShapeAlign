#include <render/shader/shader_model.hpp>
#include <render/shader/shader_colorcube.hpp>

class ModelCanvas : public nanogui::GLCanvas {
public:
    ModelCanvas(Widget *parent);

    void Init(const std::string& filename);

    void setRotation(nanogui::Vector3f vRotation);

    virtual void drawGL() override;
    
private:
	std::vector<ModelShader> mShaders;
    Eigen::Vector3f mRotation;
    objl::Loader obj_model;
};
