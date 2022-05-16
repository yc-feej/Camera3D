// Copyright 2020 Deeproute.ai. All Rights Reserved.
// Author: Yuchen Liu (yuchenliu@deeproute.ai)
#pragma once
#include <utility>
#include <vector>

#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Defines several possible options for camera movement.
// Used as abstraction to stay away from window-system specific input methods
enum class CameraMovement { FORWARD = 0, BACKWARD, LEFT, RIGHT, UP, DOWN };
enum class CameraTrackMode { MOVABLE = 0, VISUALIZER, SINGLE_FRAME };

// Default camera parameters.
const float kYaw = -90.0f;
const float kPitch = 0.0f;
const float kRoll = 0.0f;
const float kSpeed = 1.0f;
const float kSensitivity = 0.5f;

// A 3D observer camera that could be instanced and observe the world through
// its own transform matrix.
class Camera3D {
 public:
  // Since our coordinate is z-top, so the yaw movement for normal y-top
  // coordinate is actually roll. Heading radian will initialize roll and
  // look_down_radian will initialize pitch.
  Camera3D(glm::vec3 position = glm::vec3(0.0f, 0.0f, 5.0f),
           float heading_radian = 0.0f, float look_down_radian = 0.0f);

  const glm::mat4& GetMVPMatrix(float size_w, float size_h);

  // Update and return model-view matrix.
  const glm::mat4& GetModelViewMatrix() const;

  // Update and return projection metrix(we use perspective matrix here).
  const glm::mat4& GetProjectionMatrix(float size_w, float size_h);

  // Get screen UV value (between -1.0 and 1.0) of a specific position. Return
  // an invalid val if not fulfilled.
  const std::pair<float, float> GetUVAtPos(const glm::vec3& pos);

  // Processes input received from any keyboard-like input system.
  // Accepts input parameter in the form of camera defined ENUM (to abstract it
  // from windowing systems)
  void ProcessKeyboard(CameraMovement direction, float delta_time);

  // Processes input received from a mouse input system. Expects the offset
  // value in both the x and y direction. We constrain the pitch if it's out of
  // bound so that screen won't get flipped.
  void ProcessMouseMovement(float xoffset, float yoffset);

  const glm::vec3& get_position() const { return position_; }
  void set_position(const glm::vec3& position) {
    if (position != position_) {
      position_ = position;
      UpdateViewMatrix();
      UpdateMVPMatrix();
    }
  }

  float get_movement_speed() const { return movement_speed_; }
  void set_movement_speed(float speed) { movement_speed_ = speed; }

  float get_sensitivity() const { return sensitivity_; }
  void set_sensitivity(float sensitivity) { sensitivity_ = sensitivity; }

  const CameraTrackMode& get_track_mode() const { return track_mode_; }
  void set_track_mode(const CameraTrackMode& track_mode) {
    track_mode_ = track_mode;
  }

 private:
  // Calculates the front vector from the Camera's (updated) Euler Angles
  void UpdateViewMatrix();
  void UpdateProjectionMatrix(float size_w, float size_h);
  void UpdateMVPMatrix(float size_w, float size_h);
  void UpdateMVPMatrix();

  // Camera Attributes
  mutable glm::vec3 position_;
  mutable glm::mat4 projection_mat_;
  mutable glm::mat4 view_mat_;
  mutable glm::mat4 mvp_mat_;

  // Vector attributes
  glm::vec3 front_;
  glm::vec3 right_;
  glm::vec3 up_;

  // Window size
  float size_w_ = 1920.0f;
  float size_h_ = 1080.0f;

  // Euler Angles
  float yaw_;
  float pitch_;
  float roll_;

  // Camera options
  float movement_speed_ = kSpeed;
  float sensitivity_ = kSensitivity;

  // Visibility
  CameraTrackMode track_mode_ = CameraTrackMode::MOVABLE;
};
