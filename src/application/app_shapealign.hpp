#include <render/canvas/canvas_model.hpp>

class ShapeAlignApplication : public nanogui::Screen {
public:
    ShapeAlignApplication();
    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);
    virtual void draw(NVGcontext *ctx);
    
private:
    ModelCanvas *mCanvas;
};
