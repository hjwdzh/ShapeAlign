#include <application/app_shapealign.hpp>
#include <storage/persistent.hpp>


int main(int argc, char ** argv) {
    try {
        nanogui::init();
        
        {
            nanogui::ref<ShapeAlignApplication> app = new ShapeAlignApplication();
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

