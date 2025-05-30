#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include <string>

enum class ShaderType : uint8_t {
  Vertex,
  Fragment,
};

class Shader {
private:
  constexpr static int INFO_BUF_SIZE = 512;
  bool is_delete = false;

  std::string read_file(std::string_view file_path);
  int compile_shader(ShaderType shader_type, const char* shader_code);
  void link_shader(unsigned int& shader_id, unsigned int vertex, unsigned int fragment);

public:
  unsigned int ID;

  Shader(std::string_view vertex_path, std::string_view fragment_path);
  ~Shader();
  void use();

  void set_bool(std::string_view name, bool value) const;
  void set_int(std::string_view name, int value) const;
  void set_float(std::string_view name, float value) const;
  void set_vec3(std::string_view name, float x, float y, float z) const;
  void set_vec3(std::string_view name, const glm::vec3& vec) const;
  void set_mat4(std::string_view name, const glm::mat4& martix) const;

  void clear();
};
