#pragma once

#include <glad/glad.h>

#include <string_view>

enum class TextureFormat : uint8_t {
  RGB,
  RGBA,
  RED,
};

struct TextureArgs {
  std::string_view uniform_name;
  std::string_view path;
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

 private:
  std::string_view uniform_name_;
  GLuint texture_id_{};
  int width_{};
  int height_{};
  int nr_channels_{};
  int unit_index_{};

  std::pair<GLint, GLint> handle_format(bool auto_format, int nr_channels,
                                        TextureFormat internal_format,
                                        TextureFormat format);
  GLint texture_format(TextureFormat format);
  static int init_unit_index();
};