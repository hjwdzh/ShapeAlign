#include <application/app_colorcube.hpp>

int main(int /* argc */, char ** /* argv */) {
    try {
        nanogui::init();
        
        /* scoped variables */ {
            nanogui::ref<ColorcubeApplication> app = new ColorcubeApplication();
            app->drawAll();
            app->setVisible(true);
            nanogui::mainloop();
        }
        
        nanogui::shutdown();
    } catch (const std::runtime_error &e) {
        std::string error_msg = std::string("Caught a fatal error: ") + std::string(e.what());
        std::cerr << error_msg << std::endl;
        return -1;
    }
    
    return 0;
}
