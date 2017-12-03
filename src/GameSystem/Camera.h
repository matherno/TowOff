#pragma once

#include <mathernogl/maths/Transform.h>

class Camera
  {
private:
  bool valid = false;
  mathernogl::Transform worldToCamera;
  mathernogl::Transform cameraToClip;

public:
  Camera() {};
  virtual ~Camera() {};

  mathernogl::Transform* getWorldToCamera() { return &worldToCamera; }
  mathernogl::Transform* getCameraToClip() { return &cameraToClip; }
  void setValid(bool valid){ this->valid = valid; }
  bool isValid() const { return valid; }
  };
