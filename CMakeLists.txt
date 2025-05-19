cmake_minimum_required(VERSION 3.20)
project(opengl_learn LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories("./includes")

set(SRCS
  src/main.cpp
  src/Shader.cpp
  src/glfw_wrapper.cpp
)

add_executable(${PROJECT_NAME} ${SRCS})

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

find_package(glad CONFIG REQUIRED)
target_link_libraries(${PROJECT_NAME} PRIVATE glad::glad)