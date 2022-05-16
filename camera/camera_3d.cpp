// Copyright 2020 Deeproute.ai. All Rights Reserved.
// Author: Yuchen Liu (yuchenliu@deeproute.ai)
#include "camera/camera_3d.h"

#include <cmath>
#include <iostream>

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
  // TODO

  return std::make_pair(0.0f, 0.0f);
}

void Camera3D::ProcessKeyboard(CameraMovement direction, float delta_time) {
  // TODO
}

void Camera3D::ProcessMouseMovement(float x_offset, float y_offset) {
  // TODO
}

// Calculates the front vector from the Camera's (updated) Euler Angles
void Camera3D::UpdateViewMatrix() {
  // TODO
}

void Camera3D::UpdateProjectionMatrix(float size_w, float size_h) {
  // TODO
}

void Camera3D::UpdateMVPMatrix(float size_w, float size_h) {
  UpdateProjectionMatrix(size_w, size_h);
  UpdateMVPMatrix();
}

void Camera3D::UpdateMVPMatrix() { mvp_mat_ = projection_mat_ * view_mat_; }
