#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

enum class CameraMovement {
  Forward,
  Backward,
  Left,
  Right,
};

class Camera {
 public:
  constexpr static float YAW = -90.f;
  constexpr static float PITCH = 0.0f;
  constexpr static float SPEED = 2.5f;
  constexpr static float SENSITIVITY = 0.1f;
  constexpr static float ZOOM = 45.0f;

  glm::vec3 position_;
  glm::vec3 front_;
  glm::vec3 up_;
  glm::vec3 right_;
  glm::vec3 world_up_;

  float yaw_;
  float pitch_;

  float movement_speed_;
  float mouse_sensitivity_;
  float zoom_;

  Camera(glm::vec3 position = glm::vec3{0.0f, 0.0f, 0.0f},
         glm::vec3 up = glm::vec3{0.0f, 1.0f, 0.0f}, float yaw = YAW, float pitch = PITCH);

  Camera(float pos_x, float pos_y, float pos_z, float up_x, float up_y, float up_z, float yaw,
         float pitch);

  glm::mat4 view_matrix();
  void process_keyboard(CameraMovement direction, float delta_time);
  void process_mouse_movement(float x_offset, float y_offset, GLboolean constrain_pitch = true);
  void process_mouse_scroll(float y_offset);

 private:
  void update_camera_vectors();
};