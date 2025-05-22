#include "glad_wrapper.hpp"

using namespace glad;

VertexBufferLayout::VertexBufferLayout(std::vector<VertexAttribute> attribute)
    : attribute_(std::move(attribute)) {}

auto VertexBufferLayout::begin() -> std::vector<VertexAttribute>::iterator {
  return attribute_.begin();
}

auto VertexBufferLayout::end() -> std::vector<VertexAttribute>::iterator {
  return attribute_.end();
}

auto VertexBufferLayout::cbegin() const
    -> std::vector<VertexAttribute>::const_iterator {
  return attribute_.cbegin();
}

auto VertexBufferLayout::cend() const
    -> std::vector<VertexAttribute>::const_iterator {
  return attribute_.cend();
}

VertexBuffer::VertexBuffer(std::span<float> vertices,
                           std::shared_ptr<VertexBufferLayout> layout)
    : layout_(std::move(layout)) {
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ARRAY_BUFFER, ID);
  glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float),
               vertices.data(), GL_STATIC_DRAW);

  calculate_stride();
}

VertexBuffer::~VertexBuffer() {
  glDeleteBuffers(1, &ID);
}

void VertexBuffer::bind() {
  glBindBuffer(GL_ARRAY_BUFFER, ID);
}

void VertexBuffer::unbind() {
  glBindBuffer(GL_ARRAY_BUFFER, 0);
}

auto VertexBuffer::get_vbo_layout() -> std::shared_ptr<VertexBufferLayout> {
  return layout_;
}

size_t VertexBuffer::stride() const {
  return stride_;
}

void VertexBuffer::calculate_stride() {
  for (auto attribute : *layout_) {
    stride_ += static_cast<uint8_t>(attribute.type);
  }
}

IndexBuffer::IndexBuffer(std::span<unsigned int> vertices) {
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(unsigned int),
               vertices.data(), GL_STATIC_DRAW);
  index_num_ = vertices.size();
}

IndexBuffer::~IndexBuffer() {
  glDeleteBuffers(1, &ID);
}

void IndexBuffer::bind() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
}

void IndexBuffer::unbind() {
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

size_t IndexBuffer::index_num() const {
  return index_num_;
}

VertexArray::VertexArray() {
  glGenVertexArrays(1, &ID);
}

VertexArray::~VertexArray() {
  glDeleteVertexArrays(1, &ID);
}

void VertexArray::bind() {
  glBindVertexArray(ID);
}

void VertexArray::unbind() {
  glBindVertexArray(0);
}

void VertexArray::set_vbo(std::shared_ptr<VertexBuffer> vbo) {
  vertex_buffer_ = std::move(vbo);
  config_arrtibute_pointer();
}

void VertexArray::set_vbo(std::span<float> vertices,
                          std::shared_ptr<VertexBufferLayout> layout) {
  vertex_buffer_ = std::make_shared<VertexBuffer>(vertices, layout);
  config_arrtibute_pointer();
}

void VertexArray::set_ebo(std::shared_ptr<IndexBuffer> ebo) {
  index_buffer_ = std::move(ebo);
}

void VertexArray::set_ebo(std::span<unsigned int> vertices) {
  index_buffer_ = std::make_shared<IndexBuffer>(vertices);
}

void VertexArray::config_arrtibute_pointer() {
  int offset{};
  for (auto attribute : *vertex_buffer_->get_vbo_layout()) {
    auto size = static_cast<uint8_t>(attribute.type);

    glVertexAttribPointer(attribute.index, size, GL_FLOAT,
                          attribute.is_normalize ? GL_TRUE : GL_FALSE,
                          vertex_buffer_->stride() * sizeof(float),
                          (void*)offset);

    glEnableVertexAttribArray(attribute.index);
    offset += size * sizeof(float);
  }
}

void VertexArray::draw_arrays(DrawMode mode, GLint first, GLsizei count) const {
  glDrawArrays(draw_mode_map.at(mode), first, count);
}

auto VertexArray::vbo() const -> std::shared_ptr<VertexBuffer> {
  return vertex_buffer_;
}

auto VertexArray::ebo() const -> std::shared_ptr<IndexBuffer> {
  return index_buffer_;
}