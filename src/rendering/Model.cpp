#include "Model.hpp"

#include <spdlog/spdlog.h>

Model::Model(std::string_view path, bool gamma) : gamma_correction(gamma) {
  load_model(path);
}

void Model::draw(const Shader& shader) {
  for (auto& mesh : meshes_) {
    mesh.draw(shader);
  }
}

void Model::load_model(std::string_view path) {
  Assimp::Importer importer;
  const aiScene* scene = importer.ReadFile(path.data(), aiProcess_Triangulate | aiProcess_FlipUVs);

  if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
    spdlog::error("ERROR::ASSIMP::{}", importer.GetErrorString());
    return;
  }

  directory = path.substr(0, path.find_last_of('/'));

  process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene) {
  for (uint32_t i = 0; i < node->mNumMeshes; i++) {
    aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
    meshes_.push_back(process_mesh(mesh, scene));
  }

  for (uint32_t i = 0; i < node->mNumChildren; i++) {
    process_node(node->mChildren[i], scene);
  }
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene) {
  std::vector<Vertex> vertices;
  vertices.reserve(mesh->mNumVertices);
  std::vector<unsigned int> indices;
  std::vector<std::shared_ptr<Texture>> textures;

  for (uint32_t i = 0; i < mesh->mNumVertices; i++) {
    Vertex vertex{};
    // position
    glm::vec3 vector{
      mesh->mVertices[i].x,
      mesh->mVertices[i].y,
      mesh->mVertices[i].z,
    };
    vertex.Position = vector;

    // normal
    if (mesh->HasNormals()) {
      vector.x = mesh->mNormals[i].x;
      vector.y = mesh->mNormals[i].y;
      vector.z = mesh->mNormals[i].z;
      vertex.Normal = vector;
    }

    // texture coordiantes
    if (mesh->mTextureCoords[0]) {
      glm::vec2 vec{
        mesh->mTextureCoords[0][i].x,
        mesh->mTextureCoords[0][i].y,
      };
      vertex.TexCoords = vec;
    } else {
      vertex.TexCoords = glm::vec2{0.0f, 0.0f};
    }

    vertices.push_back(vertex);
  }

  // indices
  for (uint32_t i = 0; i < mesh->mNumFaces; i++) {
    aiFace face = mesh->mFaces[i];
    for (uint32_t j = 0; j < face.mNumIndices; j++) {
      indices.push_back(face.mIndices[j]);
    }
  }

  // material
  aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

  auto diffuse_maps = load_material_textures(material, aiTextureType_DIFFUSE);
  textures.insert(textures.end(), diffuse_maps.begin(), diffuse_maps.end());
  auto specular_maps = load_material_textures(material, aiTextureType_SPECULAR);
  textures.insert(textures.end(), specular_maps.begin(), specular_maps.end());
  auto normal_maps = load_material_textures(material, aiTextureType_NORMALS);
  textures.insert(textures.end(), normal_maps.begin(), normal_maps.end());
  auto height_maps = load_material_textures(material, aiTextureType_HEIGHT);
  textures.insert(textures.end(), height_maps.begin(), height_maps.end());

  return Mesh{vertices, indices, textures};
}

auto Model::load_material_textures(aiMaterial* mat, aiTextureType type) -> std::vector<std::shared_ptr<Texture>> {
  std::vector<std::shared_ptr<Texture>> textures;
  auto texture_count = mat->GetTextureCount(type);
  textures.reserve(texture_count);
  for (uint32_t i = 0; i < texture_count; i++) {
    aiString str;
    mat->GetTexture(type, i, &str);

    bool skip = false;
    for (uint32_t j = 0; j < textures_loaded_.size(); j++) {
      if (textures_loaded_[j]->cmp_path() == str.C_Str()) {
        textures.push_back(textures_loaded_[j]);
        skip = true;
        break;
      }
    }

    if (!skip) {
      auto texture = std::make_shared<Texture>(
        TextureArgs{
          .uniform_name = std::format("{}{}", uniform_name_prefix(type), i + 1),
          .load_path = std::format("{}/{}", directory, str.C_Str()),
          .cmp_path = str.C_Str(),
          .texture_type = texture_type(type),
          .auto_format = true,
          .min_filter = GL_LINEAR_MIPMAP_LINEAR
        }
      );
      textures.push_back(texture);
      textures_loaded_.push_back(texture);
    }
  }

  return textures;
}

std::string_view Model::uniform_name_prefix(aiTextureType type) {
  switch (type) {
    case aiTextureType_DIFFUSE:
      return "texture_diffuse";
    case aiTextureType_SPECULAR:
      return "texture_specular";
    case aiTextureType_NORMALS:
      return "texture_normal";
    case aiTextureType_HEIGHT:
      return "texture_height";
    default:
      std::unreachable();
  }
}

TextureType Model::texture_type(aiTextureType type) {
  switch (type) {
    case aiTextureType_DIFFUSE:
      return TextureType::Diffuse;
    case aiTextureType_SPECULAR:
      return TextureType::Specular;
    case aiTextureType_NORMALS:
      return TextureType::Normal;
    case aiTextureType_HEIGHT:
      return TextureType::Height;
    default:
      std::unreachable();
  }
}
