#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <memory>

#include "Shader.hpp"
#include "glad_wrapper.hpp"
#include "Texture.hpp"

struct Vertex {
  glm::vec3 position;
  glm::vec3 normal;
  glm::vec2 texCoords;
};

class Mesh {
 public:
  std::vector<Vertex> vertices{};
  std::vector<unsigned int> indices{};
  std::vector<Texture> textures{};

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       std::vector<Texture> textures);
  void draw(const Shader& shader);

 private:
  std::unique_ptr<glad::VertexArray<Vertex>> vao_{};
  void setup_mesh();
};