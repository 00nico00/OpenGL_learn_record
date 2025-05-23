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

    GLint internal_format = texture_format(args.internal_format);
    GLint format = texture_format(args.format);
    unit_index = init_unit_index();

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
  glActiveTexture(GL_TEXTURE0 + unit_index);
  glBindTexture(GL_TEXTURE_2D, texture_id_);
}

GLuint Texture::id() const {
  return texture_id_;
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