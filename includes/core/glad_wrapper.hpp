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
template <typename T>
class VertexBuffer {
 public:
  VertexBuffer(std::span<T> vertices, std::shared_ptr<VertexBufferLayout> layout)
      : layout_(std::move(layout)) {
    glGenBuffers(1, &ID);
    glBindBuffer(GL_ARRAY_BUFFER, ID);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(T), vertices.data(), GL_STATIC_DRAW);

    calculate_stride();
  }

  ~VertexBuffer() { glDeleteBuffers(1, &ID); }

  void bind() { glBindBuffer(GL_ARRAY_BUFFER, ID); }
  void unbind() { glBindBuffer(GL_ARRAY_BUFFER, 0); }

  auto get_vbo_layout() -> std::shared_ptr<VertexBufferLayout> { return layout_; }
  size_t stride() const { return stride_; }

 private:
  unsigned int ID{};
  size_t stride_{};
  std::shared_ptr<VertexBufferLayout> layout_{nullptr};

  void calculate_stride() {
    for (auto attribute : *layout_) {
      stride_ += static_cast<uint8_t>(attribute.type);
    }
  }
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
template <typename T>
class VertexArray {
 public:
  VertexArray() { glGenVertexArrays(1, &ID); }
  ~VertexArray() { glDeleteVertexArrays(1, &ID); }

  void bind() { glBindVertexArray(ID); }
  void unbind() { glBindVertexArray(0); }

  void set_vbo(std::shared_ptr<VertexBuffer<T>> vbo) {
    vertex_buffer_ = std::move(vbo);
    config_arrtibute_pointer();
  }
  void set_vbo(std::span<T> vertices, std::shared_ptr<VertexBufferLayout> layout) {
    vertex_buffer_ = std::make_shared<VertexBuffer<T>>(vertices, layout);
    config_arrtibute_pointer();
  }

  void set_ebo(std::shared_ptr<IndexBuffer> ebo) { index_buffer_ = std::move(ebo); }
  void set_ebo(std::span<unsigned int> vertices) {
    index_buffer_ = std::make_shared<IndexBuffer>(vertices);
  }

  void draw_arrays(DrawMode mode, GLint first, GLsizei count) const {
    glDrawArrays(draw_mode_map.at(mode), first, count);
  }

  auto vbo() const -> std::shared_ptr<VertexBuffer<T>> { return vertex_buffer_; }
  auto ebo() const -> std::shared_ptr<IndexBuffer> { return index_buffer_; }

 private:
  unsigned int ID{};
  std::shared_ptr<VertexBuffer<T>> vertex_buffer_{nullptr};
  std::shared_ptr<IndexBuffer> index_buffer_{nullptr};

  inline const static std::unordered_map<DrawMode, GLenum> draw_mode_map{
      {DrawMode::Triangles, GL_TRIANGLES},
  };

  void config_arrtibute_pointer() {
    int offset{};
    for (auto attribute : *vertex_buffer_->get_vbo_layout()) {
      auto size = static_cast<uint8_t>(attribute.type);

      glVertexAttribPointer(attribute.index, size, GL_FLOAT,
                            attribute.is_normalize ? GL_TRUE : GL_FALSE,
                            vertex_buffer_->stride() * sizeof(float), (void*)offset);

      glEnableVertexAttribArray(attribute.index);
      offset += size * sizeof(float);
    }
  }
};

}  // namespace glad