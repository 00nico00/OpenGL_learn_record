#include "glad_wrapper.hpp"

using namespace glad;

VertexBufferLayout::VertexBufferLayout(std::vector<VertexAttribute> attribute) :
  attribute_(std::move(attribute)) {}

auto VertexBufferLayout::begin() -> std::vector<VertexAttribute>::iterator {
  return attribute_.begin();
}

auto VertexBufferLayout::end() -> std::vector<VertexAttribute>::iterator {
  return attribute_.end();
}

auto VertexBufferLayout::cbegin() const -> std::vector<VertexAttribute>::const_iterator {
  return attribute_.cbegin();
}

auto VertexBufferLayout::cend() const -> std::vector<VertexAttribute>::const_iterator {
  return attribute_.cend();
}

IndexBuffer::IndexBuffer(std::span<unsigned int> vertices) {
  glGenBuffers(1, &ID);
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, vertices.size() * sizeof(unsigned int), vertices.data(),
               GL_STATIC_DRAW);
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

void glad::enable_depth_test() {
  glEnable(GL_DEPTH_TEST);
}
