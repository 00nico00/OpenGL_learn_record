#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <stb_image.h>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <string_view>
#include <vector>

#include "Shader.hpp"
#include "Mesh.hpp"

class Model {
public:
  explicit Model(std::string_view path, bool gamma = false);

  void draw(const Shader& shader);

private:
  std::vector<std::shared_ptr<Texture>> textures_loaded_;
  std::vector<Mesh> meshes_;
  std::string_view directory;
  bool gamma_correction{};

  void load_model(std::string_view path);
  void process_node(aiNode* node, const aiScene* scene);
  Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
  auto load_material_textures(aiMaterial* mat, aiTextureType type) -> std::vector<std::shared_ptr<Texture>>;

  static std::string_view uniform_name_prefix(aiTextureType type);
  static TextureType texture_type(aiTextureType type);
};
