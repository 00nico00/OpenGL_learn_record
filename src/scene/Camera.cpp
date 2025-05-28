#include "Camera.hpp"

Camera::Camera(glm::vec3 position, glm::vec3 up, float yaw, float pitch)
    : front_(glm::vec3{0.0f, 0.0f, -1.0f}),
      movement_speed_(SPEED),
      mouse_sensitivity_(SENSITIVITY),
      zoom_(ZOOM) {
  position_ = position;
  world_up_ = up;
  yaw_ = yaw;
  pitch = pitch;
  update_camera_vectors();
}

Camera::Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw,
               float pitch)
    : front_(glm::vec3{0.0f, 0.0f, -1.0f}),
      movement_speed_(SPEED),
      mouse_sensitivity_(SENSITIVITY),
      zoom_(ZOOM) {
  position_ = glm::vec3{pos_x, pos_y, pos_z};
  world_up_ = glm::vec3{up_x, up_y, up_z};
  yaw_ = yaw;
  pitch_ = pitch;
  update_camera_vectors();
}

glm::mat4 Camera::view_matrix() {
  return glm::lookAt(position_, position_ + front_, up_);
}

void Camera::process_keyboard(CameraMovement direction, float delta_time) {
  float velocity = movement_speed_ * delta_time;
  if (direction == CameraMovement::Backward)
    position_ -= front_ * velocity;
  if (direction == CameraMovement::Forward)
    position_ += front_ * velocity;
  if (direction == CameraMovement::Left)
    position_ -= right_ * velocity;
  if (direction == CameraMovement::Right)
    position_ += right_ * velocity;
}

void Camera::process_mouse_movement(float x_offset, float y_offset, GLboolean constrain_pitch) {
  x_offset *= mouse_sensitivity_;
  y_offset *= mouse_sensitivity_;

  yaw_ += x_offset;
  pitch_ += y_offset;

  if (constrain_pitch) {
    pitch_ = glm::clamp(pitch_, -89.0f, 89.0f);
  }

  update_camera_vectors();
}

void Camera::process_mouse_scroll(float y_offset) {
  zoom_ -= y_offset;
  zoom_ = glm::clamp(zoom_, 1.0f, 45.0f);
}

void Camera::update_camera_vectors() {
  glm::vec3 front;
  front.x = std::cos(glm::radians(pitch_)) * std::cos(glm::radians(yaw_));
  front.y = std::sin(glm::radians(pitch_));
  front.z = std::cos(glm::radians(pitch_)) * std::sin(glm::radians(yaw_));
  front_ = glm::normalize(front);

  right_ = glm::normalize(glm::cross(front_, world_up_));
  up_ = glm::normalize(glm::cross(right_, front_));
}