cmake_minimum_required(VERSION 3.1)
project(ShapeAlign)

find_package(OpenGL REQUIRED)
find_package(OpenCV REQUIRED)
find_package(Ceres REQUIRED)

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG}")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")  # enable assert
set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS}")
set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG}")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fopenmp -Wno-int-in-bool-context")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -fsanitize=address")
    set(CMAKE_LINKER_FLAGS "${CMAKE_LINKER_FLAGS}")
    set(CMAKE_LINKER_FLAGS_DEBUG "${CMAKE_LINKER_FLAGS_DEBUG} -fsanitize=address")
endif()

include_directories(src)
include_directories(3rd)
include_directories(3rd/nanogui)
include_directories(3rd/eigen3)
include_directories(OpenCV_INCLUDE_DIR)
include_directories(CERES_INCLUDE_DIRS)

link_directories(3rd)

set(
    ShapeAlign_SRC
    src/main.cpp
    src/nanogui_header.hpp
    src/render/gltexture.cpp
    src/render/gltexture.hpp
    src/render/shader/shader.hpp
    src/render/shader/shader_model.cpp
    src/render/shader/shader_model.hpp
    src/render/shader/shader_image.cpp
    src/render/shader/shader_image.hpp
    src/render/canvas/canvas_model.hpp
    src/render/canvas/canvas_model.cpp
    src/render/canvas/canvas_image.hpp
    src/render/canvas/canvas_image.cpp
    src/application/app_shapealign.hpp
    src/application/app_shapealign.cpp
    src/storage/objloader.hpp
    src/storage/persistent.hpp
    src/storage/persistent.cpp
    src/storage/sens.hpp
    src/storage/sens.cpp
    src/storage/pose.hpp
    src/storage/pose.cpp
)

add_executable(
    ShapeAlign
    ${ShapeAlign_SRC}
)

target_link_libraries(
    ShapeAlign
    nanogui
    ${OPENGL_LIBRARIES}
    ${OpenCV_LIBRARIES}
    ${CERES_LIBRARIES}
)
