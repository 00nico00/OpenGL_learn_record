#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<Texture> textures)
  : vertices(std::move(vertices)), indices(std::move(indices)), textures(std::move(textures)) {
  setup_mesh();
}

void Mesh::setup_mesh() {
  auto layout = std::make_shared<glad::VertexBufferLayout>(
    std::vector<glad::VertexAttribute>{
      {0, "Position", glad::ArrtibuteType::Position},
      {1, "Normal", glad::ArrtibuteType::Normal},
      {2, "TexCoords", glad::ArrtibuteType::TexCoords}
    });

  vao_ = std::make_unique<glad::VertexArray<Vertex>>();
  vao_->bind();
  vao_->set_vbo(vertices, layout);
  vao_->set_ebo(indices);

  vao_->unbind();
}

void Mesh::draw(const Shader& shader) {
  for (int i = 0; i < textures.size(); i++) {
    auto& texture = textures[i];
    texture.bind();
    shader.set_int(texture.unform_name(), texture.unit_index());
  }
  glActiveTexture(GL_TEXTURE0);

  vao_->bind();
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  vao_->unbind();
}
