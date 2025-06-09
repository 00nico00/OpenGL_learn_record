#include "Mesh.hpp"

Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices,
           std::vector<std::shared_ptr<Texture>> textures)
  : vertices(std::move(vertices)), indices(std::move(indices)), textures(std::move(textures)) {
  setup_mesh();
}

void Mesh::setup_mesh() {
  auto layout = std::make_shared<glad::VertexBufferLayout>(
    std::vector<glad::VertexAttribute>{
      {0, "Position", glad::ArrtibuteType::Position},
      {1, "Normal", glad::ArrtibuteType::Normal},
      {2, "TexCoords", glad::ArrtibuteType::TexCoords},
      {3, "Tangent", glad::ArrtibuteType::Tangent},
      {4, "Bitangent", glad::ArrtibuteType::Bitangent},
      {5, "BoneID", glad::ArrtibuteType::BonesID, false, GL_INT},
      {6, "Weight", glad::ArrtibuteType::Weight}
    });

  vao_ = std::make_unique<glad::VertexArray<Vertex>>();
  vao_->bind();
  vao_->set_vbo(vertices, layout);
  vao_->set_ebo(indices);

  vao_->unbind();
}

void Mesh::draw(const Shader& shader) {
  for (auto& texture : textures) {
    texture->bind();
    shader.set_int(texture->unform_name(), texture->unit_index());
  }
  vao_->bind();
  glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
  vao_->unbind();

  glActiveTexture(GL_TEXTURE0);
}
