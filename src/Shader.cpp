#include "Shader.hpp"

#include "glm/gtc/type_ptr.hpp"

#include <fstream>
#include <format>
#include <sstream>
#include <print>

Shader::~Shader() {
  clear();
}

Shader::Shader(std::string_view vertex_path, std::string_view fragment_path) {
  std::string vertex_code{};
  std::string fragment_code{};
  try {
    vertex_code = read_file(vertex_path);
    fragment_code = read_file(fragment_path);
  } catch (const std::exception& e) {
    std::println(stderr, "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: {}",
                 e.what());
    return;
  }

  unsigned int vertex;
  unsigned int fragment;

  try {
    vertex = compile_shader(ShaderType::Vertex, vertex_code.data());
  } catch (std::exception& e) {
    std::println(stderr, "ERROR::SHADER::VERTEX::COMPILATION_FAILED:\n{}",
                 e.what());
    return;
  }

  try {
    fragment = compile_shader(ShaderType::Fragment, fragment_code.c_str());
  } catch (std::exception& e) {
    std::println(stderr, "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED:\n{}",
                 e.what());
    return;
  }

  try {
    link_shader(ID, vertex, fragment);
  } catch (std::exception& e) {
    std::println(stderr, "ERROR::SHADER::PROGRAM::LINKING_FAILED:\n{}",
                 e.what());
    return;
  }

  glDeleteShader(vertex);
  glDeleteShader(fragment);
};

std::string Shader::read_file(std::string_view file_path) {
  std::ifstream file{file_path.data()};
  if (!file.is_open()) {
    throw std::runtime_error(std::format("Failed to open file: {}", file_path));
  }

  std::stringstream buf{};
  buf << file.rdbuf();

  if (file.bad()) {
    throw std::runtime_error("Error reading file contents");
  }

  return buf.str();
}

int Shader::compile_shader(ShaderType shader_type, const char* shader_code) {
  GLenum type;
  switch (shader_type) {
    case ShaderType::Vertex:
      type = GL_VERTEX_SHADER;
      break;
    case ShaderType::Fragment:
      type = GL_FRAGMENT_SHADER;
      break;
  }

  unsigned int shader = glCreateShader(type);
  int success;
  char info_log[INFO_BUF_SIZE];
  glShaderSource(shader, 1, &shader_code, nullptr);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader, sizeof(info_log), nullptr, info_log);
    throw std::runtime_error(info_log);
  }

  return shader;
}

void Shader::link_shader(unsigned int& shader_id, unsigned int vertex,
                         unsigned int fragment) {
  shader_id = glCreateProgram();
  glAttachShader(shader_id, vertex);
  glAttachShader(shader_id, fragment);
  glLinkProgram(shader_id);

  int success;
  char info_log[INFO_BUF_SIZE];
  glGetProgramiv(shader_id, GL_LINK_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shader_id, sizeof(info_log), nullptr, info_log);
    throw std::runtime_error(info_log);
  }
}

void Shader::use() {
  glUseProgram(ID);
}

void Shader::set_bool(std::string_view name, bool value) const {
  glUniform1i(glGetUniformLocation(ID, name.data()), static_cast<int>(value));
}

void Shader::set_int(std::string_view name, int value) const {
  glUniform1i(glGetUniformLocation(ID, name.data()), value);
}

void Shader::set_float(std::string_view name, float value) const {
  glUniform1f(glGetUniformLocation(ID, name.data()), value);
}

void Shader::set_vec3(std::string_view name, float x, float y, float z) const {
  glUniform3f(glGetUniformLocation(ID, name.data()), x, y, z);
}

void Shader::set_mat4(std::string_view name, const glm::mat4& martix) const {
  glUniformMatrix4fv(glGetUniformLocation(ID, name.data()), 1, GL_FALSE, glm::value_ptr(martix));
}

void Shader::clear() {
  if (!is_delete) {
    glDeleteProgram(ID);
  }
}