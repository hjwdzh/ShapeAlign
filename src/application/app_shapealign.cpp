#include "app_shapealign.hpp"
#include <storage/objdata.hpp>

ShapeAlignApplication* g_app = 0;
ShapeAlignApplication::ShapeAlignApplication()
: nanogui::Screen(Eigen::Vector2i(1500, 750), "ShapeAlign GUI", false) {
    using namespace nanogui;
    
    window = new Window(this, "Model View");
    window->setPosition(Vector2i(0, 0));
    window->setLayout(new GridLayout());

    tools = new Widget(window);
    tools->setLayout(new BoxLayout(Orientation::Vertical,
                                   Alignment::Middle, 0, 5));
    g_app = this;
    Button *b0 = new Button(tools, "Add Object");
    b0->setCallback([this]() {
        std::string filename = file_dialog({ {"obj", "3D model"} }, false);
        if (OBJData::GetElement(filename) == 0) {
            filenames.push_back(filename);
            buttons.push_back(new Button(g_app->tools, GetFile(filename)));
            buttons.back()->setFlags(Button::ToggleButton);
            buttons.back()->setPushed(true);
            buttons.back()->setChangeCallback([](bool state) {
                for (int i = 0; i < g_app->buttons.size(); ++i) {
                    if (g_app->buttons[i]->pushed()) {
                        OBJData::GetElement(g_app->filenames[i])->selected = 1;
                    } else {
                        OBJData::GetElement(g_app->filenames[i])->selected = 0;
                    }
                }
                printf("\n");
            });
        }
        if (!filename.empty()) {
            mCanvas->AddElement(filename);
            OBJData::GetElement(filename)->selected = 1;
        }
        performLayout();
    });
    
    Button *b1 = new Button(tools, "Remove Object");
    b1->setCallback([this]() {
        std::vector<std::string> removed;
        int top = 0;
        for (int i = 0; i < g_app->buttons.size(); ++i) {
            if (g_app->buttons[i]->pushed()) {
                //delete buttons[i];
                removed.push_back(g_app->filenames[i]);
                g_app->tools->removeChild(g_app->buttons[i]);
            } else {
                buttons[top] = buttons[i];
                filenames[top] = filenames[i];
                top += 1;
            }
        }
        for (int i = 0; i < removed.size(); ++i) {
            OBJData::RemoveElement(removed[i]);
        }
        g_app->buttons.resize(top);
        filenames.resize(top);
        performLayout();
    });
    
    mCanvas = new ModelCanvas(window);
    mCanvas->setBackgroundColor({100, 100, 100, 255});
    mCanvas->setSize({640, 480});
    
    performLayout();
}

bool ShapeAlignApplication::keyboardEvent(int key, int scancode, int action, int modifiers) {
    if (Screen::keyboardEvent(key, scancode, action, modifiers))
        return true;
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
        setVisible(false);
        return true;
    }
    return false;
}

void ShapeAlignApplication::draw(NVGcontext *ctx) {
    /* Draw the user interface */
    Screen::draw(ctx);
}
