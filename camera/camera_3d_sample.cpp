// Copyright 2020 Deeproute.ai. All Rights Reserved.
// Author: Yuchen Liu (yuchenliu@deeproute.ai)
#include <cmath>
#include <iostream>

#include "camera/camera_3d.h"

Camera3D::Camera3D(glm::vec3 position, float heading_radian,
                   float look_down_radian)
    : position_(position),
      roll_(glm::degrees(heading_radian)),
      pitch_(glm::degrees(look_down_radian)) {
  yaw_ = 0.0f;
  UpdateViewMatrix();
  UpdateMVPMatrix(size_w_, size_h_);
}

const glm::mat4& Camera3D::GetMVPMatrix(float size_w, float size_h) {
  UpdateMVPMatrix(size_w, size_h);
  return mvp_mat_;
}

// Returns the view matrix using the LookAt Matrix
const glm::mat4& Camera3D::GetModelViewMatrix() const { return view_mat_; }

const glm::mat4& Camera3D::GetProjectionMatrix(float size_w, float size_h) {
  UpdateMVPMatrix(size_w, size_h);
  return projection_mat_;
}

const std::pair<float, float> Camera3D::GetUVAtPos(const glm::vec3& pos) {
  auto pos_vec = mvp_mat_ * glm::vec4(pos, 1.0f);
  if (pos_vec.w < 0.0f) return {1.1f, 1.1f};

  pos_vec /= pos_vec.w;
  if (pos_vec.x < 1.0f && pos_vec.x > -1.0f && pos_vec.y < 1.0f &&
      pos_vec.y > -1.0f) {
    return {1.1f, 1.1f};
  }
  return std::make_pair(pos_vec.x, pos_vec.y);
}

void Camera3D::ProcessKeyboard(CameraMovement direction, float delta_time) {
  float velocity = movement_speed_ * delta_time;
  auto& view = view_mat_;
  glm::vec3 right = glm::vec3(view[0][0], view[0][1], view[0][2]);
  glm::vec3 front = glm::vec3(view[1][0], view[1][1], view[1][2]);
  glm::vec3 up = glm::vec3(-view[2][0], -view[2][1], -view[2][2]);
  switch (direction) {
    case CameraMovement::FORWARD: {
      position_ += front_ * velocity;
      break;
    }
    case CameraMovement::BACKWARD: {
      position_ -= front_ * velocity;
      break;
    }
    case CameraMovement::LEFT: {
      position_ -= right_ * velocity;
      break;
    }
    case CameraMovement::RIGHT: {
      position_ += right_ * velocity;
      break;
    }
    case CameraMovement::DOWN: {
      position_ -= up_ * velocity;
      break;
    }
    case CameraMovement::UP: {
      position_ += up_ * velocity;
      break;
    }
    default: {
      break;
    }
  }

  UpdateViewMatrix();
  UpdateMVPMatrix();
}

void Camera3D::ProcessMouseMovement(float x_offset, float y_offset) {
  x_offset *= sensitivity_;
  y_offset *= sensitivity_;

  roll_ += x_offset;
  pitch_ += y_offset;
  // Make sure that when pitch is out of bounds, screen doesn't get flipped
  if (pitch_ > 179.0f) pitch_ = 179.0f;
  if (pitch_ < 1.0f) pitch_ = 1.0f;
  roll_ = std::fmod(roll_, 360);

  UpdateViewMatrix();
  UpdateMVPMatrix();
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera3D::UpdateViewMatrix() {
  glm::mat4 rotate_x = glm::rotate(glm::mat4(1.0f), glm::radians(pitch_),
                                   glm::vec3(1.0f, 0.0f, 0.0f));
  glm::mat4 rotate_y = glm::rotate(glm::mat4(1.0f), glm::radians(yaw_),
                                   glm::vec3(0.0f, 1.0f, 0.0f));
  glm::mat4 rotate_z = glm::rotate(glm::mat4(1.0f), glm::radians(roll_),
                                   glm::vec3(0.0f, 0.0f, 1.0f));

  glm::mat4 translate = glm::translate(glm::mat4(1.0f), position_);

  glm::mat4 model = translate * rotate_z * rotate_y * rotate_x;

  right_ = glm::vec3(model[0][0], model[0][1], model[0][2]);
  up_ = glm::vec3(model[1][0], model[1][1], model[1][2]);
  front_ = glm::vec3(-model[2][0], -model[2][1], -model[2][2]);

  view_mat_ = glm::inverse(model);
}

void Camera3D::UpdateProjectionMatrix(float size_w, float size_h) {
  if (size_w == size_w_ && size_h == size_h_) return;
  size_w_ = size_w;
  size_h_ = size_h;
  // projection_mat_ = glm::ortho(0.0f, size_w_, 0.0f, size_h_, 0.0f, 50.0f);
  projection_mat_ = glm::perspective(static_cast<float>(M_PI_4),
                                     size_w_ / size_h_, 0.1f, 1000.0f);
}

void Camera3D::UpdateMVPMatrix(float size_w, float size_h) {
  UpdateProjectionMatrix(size_w, size_h);
  UpdateMVPMatrix();
}

void Camera3D::UpdateMVPMatrix() { mvp_mat_ = projection_mat_ * view_mat_; }
