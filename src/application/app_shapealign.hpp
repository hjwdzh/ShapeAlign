#include <render/canvas/canvas_model.hpp>
#include <render/canvas/canvas_image.hpp>
#include <map>

class ShapeAlignApplication : public nanogui::Screen {
public:
    ShapeAlignApplication();
    virtual bool keyboardEvent(int key, int scancode, int action, int modifiers);
    virtual void draw(NVGcontext *ctx);
    
    std::vector<Button*> buttons;
    std::vector<GLTexture> textures;
    std::vector<std::string> filenames;
    int view_model;
    int sens_count;
private:
    ModelCanvas *mCanvas;
    ImageCanvas *nCanvas;
    Window* window, *frame_window;
    
    Widget* tools, *frames, *images;
    ImagePanel *imagePanel;
    VScrollPanel *vscroll;
};
