#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string_view>
#include <functional>

namespace glfw {

class window {
 public:
  using key_callback = std::function<void(window*, int, int, int, int)>;
  using mouse_callback = std::function<void(window*, double, double)>;
  using scroll_callback = std::function<void(window*, double, double)>;
  using resize_callback = std::function<void(window*, int, int)>;

  window(std::string_view title, int width, int height);
  ~window();

  window(const window&) = delete;
  window& operator=(const window&) = delete;

  // window operations
  bool should_close() const;
  void set_should_close();
  void swap_buffers();
  void poll_events();
  void set_title(std::string_view title);
  void show_fps();
  void close();

  // callback
  void set_key_callback(const key_callback& callback);
  void set_mouse_callback(const mouse_callback& callback);
  void set_scroll_callback(const scroll_callback& callback);
  void set_resize_callback(const resize_callback& callback);

  // window properties
  int width() const;
  int height() const;
  float aspect_ratio() const;
  GLFWwindow* native_window() const;

  // check input status
  bool is_key_pressed(int key) const;
  bool is_mouse_button_pressed(int button) const;
  void get_cursor_pos(double& xpos, double& ypos) const;

 private:
  GLFWwindow* m_window{};

  int m_width{};
  int m_height{};
  std::string_view m_title{};

  key_callback m_key_callback{};
  mouse_callback m_mouse_callback{};
  scroll_callback m_scroll_callback{};
  resize_callback m_resize_callback{};

  static bool glfw_initialized;

  static void init_glfw();
  static void terminate_glfw();

  static void key_callback_wrapper(GLFWwindow* window, int key, int scancode,
                                   int action, int mods);
  static void cursor_pos_callback_wrapper(GLFWwindow* window, double xpos,
                                          double ypos);
  static void scroll_callback_wrapper(GLFWwindow* window, double xoffset,
                                      double yoffset);
  static void framebuffer_size_callback_wrapper(GLFWwindow* window, int width,
                                                int height);
};

}  // namespace glfw