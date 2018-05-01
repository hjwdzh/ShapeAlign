#include "app_shapealign.hpp"
#include <storage/persistent.hpp>

class ImageViewSelect : public ImageView
{
public:
    ImageViewSelect(Widget* parent, GLuint imageID, int _id)
    : ImageView(parent, imageID), id(_id)
    {
    }
    bool mouseButtonEvent(const Eigen::Vector2i &p, int button, bool down, int modifiers) {
        if (button == 0 && down) {
            sens_data.selected = id;
        }
        return true;
    }

    int id;
};
ShapeAlignApplication* g_app = 0;
ShapeAlignApplication::ShapeAlignApplication()
: nanogui::Screen(Eigen::Vector2i(1400, 600), "ShapeAlign GUI", false) {
    using namespace nanogui;
    
    g_app = this;

    window = new Window(this, "Tools");
    window->setPosition(Vector2i(15, 7));
    window->setLayout(new BoxLayout(Orientation::Horizontal,
                                    Alignment::Middle, 0, 5));
    
    Button *b0 = new Button(window, "Add Object");
    b0->setCallback([this]() {
        std::string filename = file_dialog({ {"obj", "3D model"} }, false);
        if (!filename.empty()) {
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
                });
            }
            if (!filename.empty()) {
                mCanvas->AddElement(filename);
                nCanvas->AddElement(filename);
                OBJData::GetElement(filename)->selected = 1;
            }
        }
        performLayout();
    });
    
    Button *b1 = new Button(window, "Remove Object");
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
    
    Button* b3 = new Button(window, "Open Sens");
    b3->setCallback([this]() {
        std::string filename = file_dialog({ {"sens", "3D scanning data"} }, false);
        textures.clear();
        if (!filename.empty()) {
            sens_data = SensData(filename);
            for (int i = 0; i < sens_data.colors.size(); ++i) {
                char buffer[1024];
                sprintf(buffer, "filename_%d", i);
                cv::Mat c;
                cv::resize(sens_data.colors[i], c, cv::Size(640, 480));
                textures.push_back(GLTexture(buffer));
                textures.back().load(c);
                auto imgview = new ImageViewSelect(images, textures.back().texture(), i);
                imgview->setFixedSize(Vector2i(64, 48));
                imgview->setScale(0.1);
            }
            int top = 0;
            for (int i = 0; i < mCanvas->mShaders.size(); ++i) {
                if (strcmp(mCanvas->mShaders[i]->filename.c_str(), "sens") == 0) {
//                    delete mCanvas->mShaders[i];
                } else {
                    mCanvas->mShaders[top++] = mCanvas->mShaders[i];
                }
            }
            mCanvas->mShaders.resize(top);
            mCanvas->AddElement("sens");
        }
        performLayout();
    });
    
    view_model = 1;
    Button *b4 = new Button(window, "Show Model in Image");
    b4->setFlags(Button::ToggleButton);
    b4->setPushed(true);
    b4->setChangeCallback([](bool state) {
        g_app->view_model = (state) ? 1 : 0;
    });
    Button *b5 = new Button(window, "Add Keypoints");
    Button *b6 = new Button(window, "Remove Keypoints");
    Button *b7 = new Button(window, "Toggle Camera Transform");
    Button *b8 = new Button(window, "Toggle Model Transform");
    Button *b9 = new Button(window, "Save Transform");
    window = new Window(this, "Model View");
    window->setPosition(Vector2i(15, 75));
    window->setLayout(new GridLayout());

    tools = new Widget(window);
    tools->setFixedSize(nanogui::Vector2i(128, 480));
    tools->setLayout(new BoxLayout(Orientation::Vertical,
                                   Alignment::Middle, 0, 5));
    

    mCanvas = new ModelCanvas(window);
    mCanvas->setBackgroundColor({100, 100, 100, 255});
    mCanvas->setSize({480, 480});
    
    frame_window = new Window(this, "Sens View");
    
    frame_window->setLayout(new GridLayout());
    frame_window->setPosition(Vector2i(650, 75));
    
    frames = new Widget(frame_window);
    
    frames->setLayout(new BoxLayout(Orientation::Vertical,
                                   Alignment::Middle, 0, 5));
    
    vscroll = new VScrollPanel(frames);
    vscroll->setFixedSize(nanogui::Vector2i(96, 480));
    images = new Widget(vscroll);
    images->setLayout(new nanogui::BoxLayout(nanogui::Orientation::Vertical, nanogui::Alignment::Middle, 0, 0));

    nCanvas = new ImageCanvas(frame_window);
    nCanvas->setBackgroundColor({100, 100, 100, 255});
    nCanvas->setSize({640, 480});

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
