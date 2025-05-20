#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <print>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.hpp"
#include "glfw_wrapper.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static float mix_value = 0.2;

int main() {
  glfw::window window{"Learn OpenGL", 800, 600};

  window.set_key_callback(
      [](glfw::window* self, int key, int scancode, int action, int mods) {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
          self->set_should_close();

        if (key == GLFW_KEY_UP && action == GLFW_PRESS)
          mix_value = 0.2;

        if (key == GLFW_KEY_DOWN && action == GLFW_PRESS)
          mix_value = 0.8;
      });

  if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress))) {
    std::println("Failed to initialize GLAD");
    return -1;
  }

  glm::mat4 model{1.0f};
  glm::mat4 view{1.0f};
  view = glm::translate(view, glm::vec3(0.0, 0.0, -3.0f));

  glm::mat4 projection = glm::perspective(glm::radians(45.0f),
                                          window.aspect_ratio(), 0.1f, 100.0f);

  glEnable(GL_DEPTH_TEST);

  Shader our_shader{"F:\\cpp\\opengl_learn\\shader\\vertex.sf",
                    "F:\\cpp\\opengl_learn\\shader\\fragment.sf"};

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

  unsigned int indices[] = {
      0, 1, 3,  // first triangle
      1, 2, 3   // second triangle
  };

  unsigned int VBO, VAO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices,
               GL_STATIC_DRAW);

  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // texture coord attribute
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float),
                        (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);

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
    glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);

    model = glm::rotate(model, (float)glfwGetTime() * 0.01f,
                        glm::vec3(0.5f, 1.0f, 0.0f));

    our_shader.use();
    our_shader.set_float("mixValue", mix_value);
    our_shader.set_mat4("model", model);
    our_shader.set_mat4("view", view);
    our_shader.set_mat4("projection", projection);

    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);

    window.swap_buffers();
    window.poll_events();
    window.show_fps();
  }

  glDeleteVertexArrays(1, &VAO);
  glDeleteBuffers(1, &VBO);
  glDeleteBuffers(1, &EBO);

  return 0;
}