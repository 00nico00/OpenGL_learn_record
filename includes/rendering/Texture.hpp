#pragma once

#include <assimp/types.h>
#include <glad/glad.h>

#include <string>

enum class TextureFormat : uint8_t {
  RGB,
  RGBA,
  RED,
};

enum class TextureType {
  Diffuse,
  Specular,
  Normal,
  Height,
};

struct TextureArgs {
  std::string uniform_name;
  std::string load_path;
  std::string cmp_path;
  TextureType texture_type;
  TextureFormat internal_format;
  TextureFormat format;
  bool auto_format = false;
  bool generate_mipmap = true;
  GLint min_filter = GL_LINEAR;
  GLint mag_filter = GL_LINEAR;
  GLint wrap_s = GL_REPEAT;
  GLint wrap_t = GL_REPEAT;
};

class Texture {
public:
  explicit Texture(TextureArgs args);
  ~Texture();

  void bind() const;
  GLuint id() const;
  int unit_index() const;
  std::string_view unform_name() const;
  TextureType texture_type() const;
  std::string_view cmp_path() const;

private:
  std::string uniform_name_;
  GLuint texture_id_{};
  TextureType texture_type_{};
  std::string load_path_{};
  std::string cmp_path_{};
  int width_{};
  int height_{};
  int nr_channels_{};
  int unit_index_{};

  std::pair<GLint, GLint> handle_format(bool auto_format, int nr_channels,
                                        TextureFormat internal_format, TextureFormat format);
  GLint texture_format(TextureFormat format);
  static int init_unit_index();
};
