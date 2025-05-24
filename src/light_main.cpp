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

  Shader lighting_shader{"../shader/light/color.vert",
                         "../shader/light/color.frag"};
  Shader lightcube_shader{"../shader/light/light_cube.vert",
                          "../shader/light/light_cube.frag"};

  // clang-format off
  float vertices[] = {
    // positions          // normals           // texture coords
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f,  0.0f, -1.0f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f,  0.0f, 1.0f,   0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f, -1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f,  0.0f,  0.0f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, -1.0f,  0.0f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f,  1.0f,  0.0f,  0.0f, 1.0f
  };

  glm::vec3 cube_positions[] = {
    glm::vec3( 0.0f,  0.0f,  0.0f),
    glm::vec3( 2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3( 2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3( 1.3f, -2.0f, -2.5f),
    glm::vec3( 1.5f,  2.0f, -2.5f),
    glm::vec3( 1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
  };
  // clang-format on

  auto layout = std::make_shared<glad::VertexBufferLayout>(
      std::vector<glad::VertexAttribute>{
          {0, "Position", glad::ArrtibuteType::Position},
          {1, "Normal", glad::ArrtibuteType::Normal},
          {2, "TexCoords", glad::ArrtibuteType::TexCoords}});

  glad::VertexArray cube_vao{};
  cube_vao.bind();
  cube_vao.set_vbo(vertices, layout);

  glad::VertexArray lightcube_vao{};
  lightcube_vao.bind();
  lightcube_vao.set_vbo(cube_vao.vbo());

  Texture diffuse_texture{TextureArgs{.path = "../Textures/container2.png",
                                      .internal_format = TextureFormat::RGBA,
                                      .format = TextureFormat::RGBA,
                                      .min_filter = GL_LINEAR_MIPMAP_LINEAR}};

  Texture specular_texture{
      TextureArgs{.path = "../Textures/container2_specular.png",
                  .internal_format = TextureFormat::RGBA,
                  .format = TextureFormat::RGBA,
                  .min_filter = GL_LINEAR_MIPMAP_LINEAR}};

  lighting_shader.use();
  lighting_shader.set_int("material.diffuse", diffuse_texture.unit_index());
  lighting_shader.set_int("material.specular", specular_texture.unit_index());

  while (!window.should_close()) {
    window.update();

    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    lighting_shader.use();
    lighting_shader.set_vec3("light.position", camera.position_);
    lighting_shader.set_vec3("light.direction", camera.front_);
    lighting_shader.set_float("light.cutOff", glm::cos(glm::radians(12.5f)));
    lighting_shader.set_float("outerCutOff", glm::cos(glm::radians(17.5f)));
    lighting_shader.set_vec3("viewPos", camera.position_);

    lighting_shader.set_float("material.shininess", 32.0f);

    lighting_shader.set_vec3("light.ambient", 0.2f, 0.2f, 0.2f);
    lighting_shader.set_vec3("light.diffuse", 0.5f, 0.5f, 0.5f);
    lighting_shader.set_vec3("light.specular", 1.0f, 1.0f, 1.0f);

    lighting_shader.set_float("light.constant", 1.0f);
    lighting_shader.set_float("light.linear", 0.09f);
    lighting_shader.set_float("light.quadratic", 0.032f);

    glm::mat4 projection = glm::perspective(
        glm::radians(camera.zoom_), window.aspect_ratio(), 0.1f, 100.0f);
    glm::mat4 view = camera.view_matrix();
    lighting_shader.set_mat4("projection", projection);
    lighting_shader.set_mat4("view", view);

    diffuse_texture.bind();
    specular_texture.bind();

    cube_vao.bind();
    for (int i = 0; i < 10; i++) {
      glm::mat4 model{1.0f};
      model = glm::translate(model, cube_positions[i]);
      float angle = 20.0f * i;
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3{1.0f, 0.3f, 0.5f});
      lighting_shader.set_mat4("model", model);

      cube_vao.draw_arrays(glad::DrawMode::Triangles, 0, 36);
    }

    lightcube_shader.use();
    lightcube_shader.set_mat4("projection", projection);
    lightcube_shader.set_mat4("view", view);
    auto model = glm::mat4{1.0f};
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