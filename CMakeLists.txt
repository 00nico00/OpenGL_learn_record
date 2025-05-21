cmake_minimum_required(VERSION 3.20)
project(opengl_learn LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories("./includes")
include_directories("./third_party")

set(COMMON_SRCS
    src/Shader.cpp
    src/glfw_wrapper.cpp
    src/Camera.cpp
    src/glad_wrapper.cpp
)

find_package(glfw3 CONFIG REQUIRED)
find_package(glad CONFIG REQUIRED)

add_executable(camera
    src/camera_main.cpp
    ${COMMON_SRCS}
)
target_link_libraries(camera PRIVATE glfw glad::glad)

add_executable(light
    src/light_main.cpp
    ${COMMON_SRCS}
)
target_link_libraries(light PRIVATE glfw glad::glad)