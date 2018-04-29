#include <render/shader/shader_colorcube.hpp>

class ColorcubeCanvas : public nanogui::GLCanvas {
public:
    ColorcubeCanvas(Widget *parent);
    
    void setRotation(nanogui::Vector3f vRotation);

    virtual void drawGL() override;
    
private:
    ColorcubeShader mShader;
    Eigen::Vector3f mRotation;
};
