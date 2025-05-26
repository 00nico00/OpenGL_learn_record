#include "Texture.hpp"

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include <stdexcept>
#include <format>

#include "utils/Unreachable.hpp"

Texture::Texture(TextureArgs args) {
  stbi_set_flip_vertically_on_load(true);
  unsigned char* data =
      stbi_load(args.path.data(), &width_, &height_, &nr_channels_, 0);

  if (data) {
    glGenTextures(1, &texture_id_);
    glBindTexture(GL_TEXTURE_2D, texture_id_);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, args.min_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, args.mag_filter);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, args.wrap_s);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, args.wrap_t);

    auto [internal_format, format] = handle_format(
        args.auto_format, nr_channels_, args.internal_format, args.format);

    unit_index_ = init_unit_index();
    uniform_name_ = args.uniform_name;

    glTexImage2D(GL_TEXTURE_2D, 0, internal_format, width_, height_, 0, format,
                 GL_UNSIGNED_BYTE, data);

    if (args.generate_mipmap) {
      glGenerateMipmap(GL_TEXTURE_2D);
    }

    stbi_image_free(data);
  } else {
    throw std::runtime_error(
        std::format("Failed to load texture: {}", args.path));
  }
}

Texture::~Texture() {
  glDeleteTextures(1, &texture_id_);
}

void Texture::bind() const {
  glActiveTexture(GL_TEXTURE0 + unit_index_);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
}

GLuint Texture::id() const {
  return texture_id_;
}

int Texture::unit_index() const {
  return unit_index_;
}

std::string_view Texture::unform_name() const {
  return uniform_name_;
}

std::pair<GLint, GLint> Texture::handle_format(bool auto_format,
                                               int nr_channels,
                                               TextureFormat internal_format,
                                               TextureFormat format) {
  if (auto_format) {
    if (nr_channels == 1)
      return {GL_RED, GL_RED};

    if (nr_channels == 3)
      return {GL_RGB, GL_RGB};

    if (nr_channels == 4)
      return {GL_RGBA, GL_RGBA};

    throw std::runtime_error(
        std::format("Unexcepted texture channel: {}", nr_channels));
  }

  return {texture_format(internal_format), texture_format(format)};
}

GLint Texture::texture_format(TextureFormat format) {
  switch (format) {
    case TextureFormat::RED:
      return GL_RED;
      break;
    case TextureFormat::RGB:
      return GL_RGB;
      break;
    case TextureFormat::RGBA:
      return GL_RGBA;
      break;
    default:
      unreachable();
  }
  unreachable();
}

int Texture::init_unit_index() {
  static int index = 0;
  return index++;
}