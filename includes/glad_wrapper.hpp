#pragma once

#include <glad/glad.h>

#include <span>
#include <string_view>
#include <vector>
#include <memory>
#include <unordered_map>

namespace glad {

enum class ArrtibuteType : uint8_t {
  // (x, y, z)
  Position = 3,
  // (r, g, b)
  Color = 3,
  Normal = 3,
  TexCoords = 2,
};

enum class DrawMode : uint8_t {
  // GL_TRIANGLES
  Triangles,
};

struct VertexAttribute {
  unsigned int index;
  std::string_view name;
  ArrtibuteType type;
  bool is_normalize{false};
};

class VertexBufferLayout {
 public:
  explicit VertexBufferLayout(std::vector<VertexAttribute> attribute);

  auto begin() -> std::vector<VertexAttribute>::iterator;
  auto end() -> std::vector<VertexAttribute>::iterator;
  auto cbegin() const -> std::vector<VertexAttribute>::const_iterator;
  auto cend() const -> std::vector<VertexAttribute>::const_iterator;

 private:
  std::vector<VertexAttribute> attribute_;
};

// VBO Wrapper
class VertexBuffer {
 public:
  VertexBuffer(std::span<float> vertices,
               std::shared_ptr<VertexBufferLayout> layout);
  ~VertexBuffer();

  void bind();
  void unbind();

  auto get_vbo_layout() -> std::shared_ptr<VertexBufferLayout>;
  size_t stride() const;

 private:
  unsigned int ID{};
  size_t stride_{};
  std::shared_ptr<VertexBufferLayout> layout_{nullptr};

  void calculate_stride();
};

// EBO Wrapper
class IndexBuffer {
 public:
  explicit IndexBuffer(std::span<unsigned int> vertices);
  ~IndexBuffer();

  void bind();
  void unbind();

  size_t index_num() const;

 private:
  unsigned int ID{};
  size_t index_num_{};
};

// VAO Wrapper
class VertexArray {
 public:
  VertexArray();
  ~VertexArray();

  void bind();
  void unbind();

  void set_vbo(std::shared_ptr<VertexBuffer> vbo);
  void set_vbo(std::span<float> vertices,
               std::shared_ptr<VertexBufferLayout> layout);

  void set_ebo(std::shared_ptr<IndexBuffer> ebo);
  void set_ebo(std::span<unsigned int> vertices);

  void draw_arrays(DrawMode mode, GLint first, GLsizei count) const;

 private:
  unsigned int ID{};
  std::shared_ptr<VertexBuffer> vertex_buffer_{nullptr};
  std::shared_ptr<IndexBuffer> index_buffer_{nullptr};

  inline const static std::unordered_map<DrawMode, GLenum> draw_mode_map{
      {DrawMode::Triangles, GL_TRIANGLES},
  };

  void config_arrtibute_pointer();
};

}  // namespace glad