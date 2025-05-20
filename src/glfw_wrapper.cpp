#include "glfw_wrapper.hpp"

#include <print>
#include <format>

using namespace glfw;

bool window::glfw_initialized = false;

window::window(std::string_view title, int width, int height)
    : m_title(title), m_width(width), m_height(height) {

  init_glfw();

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  m_window =
      glfwCreateWindow(m_width, m_height, m_title.data(), nullptr, nullptr);
  if (!m_window) {
    std::println(stderr, "Failed to create GLFW window");
    terminate_glfw();
    return;
  }

  glfwMakeContextCurrent(m_window);

  glfwSetWindowUserPointer(m_window, this);
  glfwSetKeyCallback(m_window, key_callback_wrapper);
  glfwSetCursorPosCallback(m_window, cursor_pos_callback_wrapper);
  glfwSetScrollCallback(m_window, scroll_callback_wrapper);
  glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback_wrapper);

  // glfwSwapInterval(1);
}

window::~window() {
  if (m_window) {
    glfwDestroyWindow(m_window);
  }
  terminate_glfw();
}

void window::init_glfw() {
  if (!glfw_initialized) {
    if (!glfwInit()) {
      std::println(stderr, "Failed to initialize GLFW");
      return;
    }
    glfw_initialized = true;
  }
}

void window::terminate_glfw() {
  if (glfw_initialized) {
    glfwTerminate();
    glfw_initialized = false;
  }
}

bool window::should_close() const {
  return glfwWindowShouldClose(m_window);
}

void window::set_should_close() {
  glfwSetWindowShouldClose(m_window, true);
}

void window::swap_buffers() {
  glfwSwapBuffers(m_window);
}

void window::poll_events() {
  glfwPollEvents();
}

void window::set_title(std::string_view title) {
  m_title = title;
  glfwSetWindowTitle(m_window, m_title.data());
}

void window::update() {
  float current_frame = glfwGetTime();
  delta_time = current_frame - last_frame;
  last_frame = current_frame;
}

void window::close() {
  glfwSetWindowShouldClose(m_window, true);
}

void window::set_key_callback(const key_callback& callback) {
  m_key_callback = callback;
}

void window::set_mouse_callback(const mouse_callback& callback) {
  m_mouse_callback = callback;
}

void window::set_scroll_callback(const scroll_callback& callback) {
  m_scroll_callback = callback;
}

void window::set_resize_callback(const resize_callback& callback) {
  m_resize_callback = callback;
}

int window::width() const {
  return m_width;
}

int window::height() const {
  return m_height;
}

float window::aspect_ratio() const {
  return static_cast<float>(m_width) / static_cast<float>(m_height);
}

GLFWwindow* window::native_window() const {
  return m_window;
}

bool window::is_key_pressed(int key) const {
  return glfwGetKey(m_window, key) == GLFW_PRESS;
}

bool window::is_mouse_button_pressed(int button) const {
  return glfwGetMouseButton(m_window, button) == GLFW_PRESS;
}

void window::get_cursor_pos(double& xpos, double& ypos) const {
  glfwGetCursorPos(m_window, &xpos, &ypos);
}

void window::key_callback_wrapper(GLFWwindow* glfw_window, int key,
                                  int scancode, int action, int mods) {
  if (!glfw_window)
    return;

  auto* win = static_cast<window*>(glfwGetWindowUserPointer(glfw_window));
  if (win && win->m_key_callback) {
    win->m_key_callback(win, key, scancode, action, mods);
  }
}

void window::cursor_pos_callback_wrapper(GLFWwindow* glfw_window, double xpos,
                                         double ypos) {
  if (!glfw_window)
    return;

  auto* win = static_cast<window*>(glfwGetWindowUserPointer(glfw_window));
  if (win && win->m_mouse_callback) {
    win->m_mouse_callback(win, xpos, ypos);
  }
}

void window::scroll_callback_wrapper(GLFWwindow* glfw_window, double xoffset,
                                     double yoffset) {
  if (!glfw_window)
    return;

  auto* win = static_cast<window*>(glfwGetWindowUserPointer(glfw_window));
  if (win && win->m_scroll_callback) {
    win->m_scroll_callback(win, xoffset, yoffset);
  }
}

void window::framebuffer_size_callback_wrapper(GLFWwindow* glfw_window,
                                               int width, int height) {
  if (!glfw_window)
    return;

  auto* win = static_cast<window*>(glfwGetWindowUserPointer(glfw_window));
  if (win) {
    glViewport(0, 0, width, height);

    win->m_width = width;
    win->m_height = height;
    if (win->m_resize_callback) {
      win->m_resize_callback(win, width, height);
    }
  }
}