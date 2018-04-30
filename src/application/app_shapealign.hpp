#include <render/canvas/canvas_model.hpp>
#include <map>

class ShapeAlignApplication : public nanogui::Screen {
public:
    ShapeAlignApplication();
    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);
    virtual void draw(NVGcontext *ctx);
    
    std::vector<Button*> buttons;
    std::vector<std::string> filenames;
private:
    ModelCanvas *mCanvas;
    Window* window;
    Widget* tools;
};
