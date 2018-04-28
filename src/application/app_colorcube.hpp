#include <render/canvas/canvas_colorcube.hpp>

class ColorcubeApplication : public nanogui::Screen {
public:
    ColorcubeApplication();
    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);
    virtual void draw(NVGcontext *ctx);
    
private:
    ColorcubeCanvas *mCanvas;
};
