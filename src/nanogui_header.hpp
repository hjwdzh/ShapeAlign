#ifndef NANOGUI_HEADER_H_
#define NANOGUI_HEADER_H_

#include <nanogui/opengl.h>
#include <nanogui/glutil.h>
#include <nanogui/screen.h>
#include <nanogui/window.h>
#include <nanogui/layout.h>
#include <nanogui/label.h>
#include <nanogui/checkbox.h>
#include <nanogui/button.h>
#include <nanogui/toolbutton.h>
#include <nanogui/popupbutton.h>
#include <nanogui/combobox.h>
#include <nanogui/progressbar.h>
#include <nanogui/entypo.h>
#include <nanogui/messagedialog.h>
#include <nanogui/textbox.h>
#include <nanogui/slider.h>
#include <nanogui/imagepanel.h>
#include <nanogui/imageview.h>
#include <nanogui/vscrollpanel.h>
#include <nanogui/colorwheel.h>
#include <nanogui/graph.h>
#include <nanogui/tabwidget.h>
#include <nanogui/glcanvas.h>

// Includes for the GLTexture class.
#include <cstdint>
#include <memory>
#include <utility>


#define RESOURCE_DIR std::string("/Users/jingwei/Desktop/project/ShapeAlign/resources")

inline std::string GetDirectory(std::string filename) {
    std::string directory;
    const size_t last_slash_idx = filename.rfind('\\');
    if (std::string::npos != last_slash_idx)
    {
        directory = filename.substr(0, last_slash_idx);
    } else {
        const size_t last_slash_idx = filename.rfind('/');
        if (std::string::npos != last_slash_idx)
        {
            directory = filename.substr(0, last_slash_idx);
        } else {
            directory = filename;
        }
    }
    return directory;
}

inline std::string GetFile(std::string filename) {
    std::string directory;
    const size_t last_slash_idx = filename.rfind('\\');
    if (std::string::npos != last_slash_idx)
    {
        directory = filename.substr(last_slash_idx + 1, filename.size() - last_slash_idx - 1);
    } else {
        const size_t last_slash_idx = filename.rfind('/');
        if (std::string::npos != last_slash_idx)
        {
            directory = filename.substr(last_slash_idx + 1, filename.size() - last_slash_idx - 1);
        } else {
            directory = filename;
        }
    }
    return directory;
}


#endif
