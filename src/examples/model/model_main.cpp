#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "Texture.hpp"
#include "glfw_wrapper.hpp"
#include "Camera.hpp"
#include "glad_wrapper.hpp"
#include "Model.hpp"
#include "utils/Logger.hpp"
#include "utils/Guard.hpp"

bool first_mouse = true;
float last_x = 800.0f / 2.0;
float last_y = 600.0 / 2.0;

int main() {
  Logger::init("model");
  Guard guard{[] { Logger::shutdown(); }};

  glfw::window window{"model", 800, 600};
  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    spdlog::error("Failed to initialize GLAD");
    return -1;
  }

  Camera camera{glm::vec3{0.0f, 0.0f, 3.0f}};
  window.disable_cursor();
  window.set_key_callback([](glfw::window* self, int key, int scancode, int action, int mods) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
      self->set_should_close();
  });

  window.set_update_callback([&](glfw::window* self, float delta_time) {
    if (self->is_key_pressed(GLFW_KEY_W))
      camera.process_keyboard(CameraMovement::Forward, delta_time);
    if (self->is_key_pressed(GLFW_KEY_S))
      camera.process_keyboard(CameraMovement::Backward, delta_time);
    if (self->is_key_pressed(GLFW_KEY_A))
      camera.process_keyboard(CameraMovement::Left, delta_time);
    if (self->is_key_pressed(GLFW_KEY_D))
      camera.process_keyboard(CameraMovement::Right, delta_time);
  });

  window.set_mouse_callback([&](glfw::window* self, double xpos_in, double ypos_in) {
    auto xpos = static_cast<float>(xpos_in);
    auto ypos = static_cast<float>(ypos_in);

    if (first_mouse) {
      last_x = xpos;
      last_y = ypos;
      first_mouse = false;
    }

    float x_offset = xpos - last_x;
    float y_offset = last_y - ypos;
    last_x = xpos;
    last_y = ypos;

    camera.process_mouse_movement(x_offset, y_offset);
  });

  window.set_scroll_callback([&](glfw::window* self, double x_offset, double y_offset) {
    camera.process_mouse_scroll(static_cast<float>(y_offset));
  });

  glad::enable_depth_test();

  Shader shader{
    "../../shader/model/model.vert",
    "../../shader/model/model.frag"};
  Model backpack_model{"../../resources/backpack/backpack.obj"};

  while (!window.should_close()) {
    window.update();

    glClearColor(0.05f, 0.05f, 0.05f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.use();

    glm::mat4 projection = glm::perspective(
      glm::radians(camera.zoom_),
      window.aspect_ratio(),
      0.1f,
      100.0f
      );
    glm::mat4 view = camera.view_matrix();
    shader.set_mat4("projection", projection);
    shader.set_mat4("view", view);

    // render the loaded model
    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));
    shader.set_mat4("model", model);
    backpack_model.draw(shader);

    window.swap_buffers();
    window.poll_events();
  }

  return 0;
}
