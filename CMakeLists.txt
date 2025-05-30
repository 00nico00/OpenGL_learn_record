cmake_minimum_required(VERSION 3.20)
project(opengl_learn LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (MSVC)
  add_compile_options(/utf-8)
endif ()

include_directories("./includes")
include_directories("./includes/core")
include_directories("./includes/rendering")
include_directories("./includes/scene")
include_directories("./third_party")

set(CORE_SRCS
    src/core/glfw_wrapper.cpp
    src/core/glad_wrapper.cpp
)

set(RENDERING_SRCS
    src/rendering/Texture.cpp
    src/rendering/Mesh.cpp
    src/rendering/Shader.cpp
)

set(SCENE_SRCS
    src/scene/Camera.cpp
)

set(MODEL_SRCS
    src/rendering/Model.cpp
)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)

add_executable(camera
    src/examples/camera/camera_main.cpp
    ${CORE_SRCS}
    ${RENDERING_SRCS}
    ${SCENE_SRCS}
)
target_link_libraries(camera PRIVATE glfw glad::glad)

add_executable(light
    src/examples/light/light_main.cpp
    ${CORE_SRCS}
    ${RENDERING_SRCS}
    ${SCENE_SRCS}
)
target_link_libraries(light PRIVATE glfw glad::glad)

add_executable(model
    src/examples/model/model_main.cpp
    ${CORE_SRCS}
    ${RENDERING_SRCS}
    ${SCENE_SRCS}
    ${MODEL_SRCS}
)
target_link_libraries(model PRIVATE glfw glad::glad)