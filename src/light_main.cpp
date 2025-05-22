#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "glfw_wrapper.hpp"
#include "Camera.hpp"
#include "glad_wrapper.hpp"
#include "Logger.hpp"
#include "Guard.hpp"

bool first_mouse = true;
float last_x = 800.0f / 2.0;
float last_y = 600.0 / 2.0;

glm::vec3 light_pos{1.2f, 1.0f, 2.0f};

int main() {
  Logger::init("light");
  Guard guard{[] {
    Logger::shutdown();
  }};

  glfw::window window{"light", 800, 600};
  Camera camera{glm::vec3{0.0f, 0.0f, 3.0f}};

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    spdlog::error("Failed to initialize GLAD");
    return -1;
  }

  window.disable_cursor();
  window.set_key_callback(
      [](glfw::window* self, int key, int scancode, int action, int mods) {
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

  window.set_mouse_callback(
      [&](glfw::window* self, double xpos_in, double ypos_in) {
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

  window.set_scroll_callback(
      [&](glfw::window* self, double x_offset, double y_offset) {
        camera.process_mouse_scroll(static_cast<float>(y_offset));
      });

  glEnable(GL_DEPTH_TEST);

  Shader lighting_shader{"../shader/light/color_vertex.sf",
                         "../shader/light/color_fragment.sf"};
  Shader lightcube_shader{"../shader/light/light_cube_vertex.sf",
                          "../shader/light/light_cube_fragment.sf"};

  // clang-format off
  float vertices[] = {
    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,

    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,

     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,

    -0.5f, -0.5f, -0.5f,
     0.5f, -0.5f, -0.5f,
     0.5f, -0.5f,  0.5f,
     0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,

    -0.5f,  0.5f, -0.5f,
     0.5f,  0.5f, -0.5f,
     0.5f,  0.5f,  0.5f,
     0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
  };
  // clang-format on

  auto layout = std::make_shared<glad::VertexBufferLayout>(
      std::vector<glad::VertexAttribute>{
          {0, "Position", glad::ArrtibuteType::Position}});

  glad::VertexArray cube_vao{};
  cube_vao.bind();
  cube_vao.set_vbo(vertices, layout);

  glad::VertexArray lightcube_vao{};
  lightcube_vao.bind();
  lightcube_vao.set_vbo(cube_vao.vbo());

  while (!window.should_close()) {
    window.update();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lighting_shader.use();
    lighting_shader.set_vec3("objectColor", 1.0f, 0.5f, 0.31f);
    lighting_shader.set_vec3("lightColor", 1.0f, 1.0f, 1.0f);

    glm::mat4 projection = glm::perspective(
        glm::radians(camera.zoom_), window.aspect_ratio(), 0.1f, 100.0f);
    glm::mat4 view = camera.view_matrix();
    lighting_shader.set_mat4("projection", projection);
    lighting_shader.set_mat4("view", view);

    glm::mat4 model{1.0f};
    lighting_shader.set_mat4("model", model);

    cube_vao.bind();
    cube_vao.draw_arrays(glad::DrawMode::Triangles, 0, 36);

    lightcube_shader.use();
    lightcube_shader.set_mat4("projection", projection);
    lightcube_shader.set_mat4("view", view);
    model = glm::mat4{1.0f};
    model = glm::translate(model, light_pos);
    model = glm::scale(model, glm::vec3{0.2f});
    lightcube_shader.set_mat4("model", model);

    lightcube_vao.bind();
    lightcube_vao.draw_arrays(glad::DrawMode::Triangles, 0, 36);

    window.swap_buffers();
    window.poll_events();
  }

  return 0;
}