#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <vector>
#include <memory>

#include "Shader.hpp"
#include "glad_wrapper.hpp"
#include "Texture.hpp"

constexpr int MAX_BONE_INFLUENCE = 4;

struct Vertex {
  glm::vec3 Position;
  glm::vec3 Normal;
  glm::vec2 TexCoords;
  glm::vec3 Tangent;
  glm::vec3 Bitangent;
  int m_BoneIDs[MAX_BONE_INFLUENCE];
  float w_Weights[MAX_BONE_INFLUENCE];
};

class Mesh {
public:
  std::vector<Vertex> vertices{};
  std::vector<unsigned int> indices{};
  // avoid generate the same texture id
  std::vector<std::shared_ptr<Texture>> textures{};

  Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
       std::vector<std::shared_ptr<Texture>> textures);
  void draw(const Shader& shader);

private:
  std::unique_ptr<glad::VertexArray<Vertex>> vao_{};
  void setup_mesh();
};
