#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <print>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "Shader.hpp"
#include <glad_wrapper.hpp>
#include "glfw_wrapper.hpp"
#include "Camera.hpp"

static float mix_value = 0.2;
constexpr static float speed = 2.5f;

bool first_mouse = true;
float last_x = 800.0f / 2.0;
float last_y = 600.0 / 2.0;

int main() {
  glfw::window window{"Learn OpenGL", 800, 600};
  window.disable_cursor();

  Camera camera{glm::vec3{0.0f, 0.0f, 3.0f}};

  window.set_key_callback(
      [&](glfw::window* self, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
          self->set_should_close();

        if (key == GLFW_KEY_UP && action == GLFW_PRESS)
          mix_value = 0.2;

        if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
          mix_value = 0.8;
      });

  window.set_update_callback([&](glfw::window* self, float delta_time) {
    float camera_speed = speed;
    if (self->delta_time > 0)
      camera_speed *= self->delta_time;

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

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::println("Failed to initialize GLAD");
    return -1;
  }

  glEnable(GL_DEPTH_TEST);

  Shader our_shader{"../shader/camera/vertex.sf",
                    "../shader/camera/fragment.sf"};

  // clang-format off
  float vertices[] = {
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
       0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
       0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
       0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
       0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
      -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
      -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
  };
  // clang-format on

  // clang-format off
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

  std::vector<glad::VertexAttribute> v_layout;
  v_layout.push_back(glad::VertexAttribute{
      .index = 0, .name = "Position", .type = glad::ArrtibuteType::Position});

  v_layout.push_back(glad::VertexAttribute{
      .index = 1, .name = "TexCoords", .type = glad::ArrtibuteType::TexCoords});

  glad::VertexArray vao{};
  vao.bind();

  vao.set_vbo(vertices, std::make_shared<glad::VertexBufferLayout>(v_layout));

  unsigned int texture1, texture2;
  glGenTextures(1, &texture1);
  glBindTexture(GL_TEXTURE_2D, texture1);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  int width, height, nr_channels;
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data =
      stbi_load("F:\\cpp\\opengl_learn\\Textures\\container.jpg", &width,
                &height, &nr_channels, 0);

  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::println(stderr, "Failed to load texture");
  }
  stbi_image_free(data);

  glGenTextures(1, &texture2);
  glBindTexture(GL_TEXTURE_2D, texture2);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

  data = stbi_load("F:\\cpp\\opengl_learn\\Textures\\awesomeface.png", &width,
                   &height, &nr_channels, 0);
  if (data) {
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA,
                 GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
  } else {
    std::println(stderr, "Failed to load texture");
  }
  stbi_image_free(data);

  our_shader.use();
  our_shader.set_int("texture1", 0);
  our_shader.set_int("texture2", 1);

  while (!window.should_close()) {
    window.update();
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    our_shader.use();
    our_shader.set_float("mixValue", mix_value);

    glm::mat4 projection = glm::perspective(
        glm::radians(camera.zoom_), window.aspect_ratio(), 0.1f, 100.0f);
    our_shader.set_mat4("projection", projection);

    our_shader.set_mat4("view", camera.view_matrix());

    vao.bind();
    for (uint32_t i = 0; i < 10; i++) {
      glm::mat4 model{1.0f};
      model = glm::translate(model, cube_positions[i]);
      float angle = 20.0f * i;
      model =
          glm::rotate(model, glm::radians(angle), glm::vec3(1.0f, 0.3f, 0.5f));
      our_shader.set_mat4("model", model);

      vao.draw_arrays(glad::DrawMode::Triangles, 0, 36);
    }

    window.swap_buffers();
    window.poll_events();
  }

  return 0;
}