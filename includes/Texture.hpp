#pragma once

#include <glad/glad.h>

#include <string_view>

enum class TextureFormat : uint8_t {
  RGB,
  RGBA,
  RED,
};

struct TextureArgs {
  std::string_view path;
  TextureFormat internal_format;
  TextureFormat format;
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

 private:
  GLuint texture_id_{};
  int width_{};
  int height_{};
  int nr_channels_{};
  int unit_index{};

  GLint texture_format(TextureFormat format);
  static int init_unit_index();
};