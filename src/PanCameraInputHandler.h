#pragma once

#include <src/GameSystem/GameSystem.h>

/*
*   
*/

class PanCameraInputHandler : public InputHandler
  {
private:
  Vector3D position;
  float panSpeed = 50;
  float yawSpeed = 90;
  float yaw;
  float pitch;
  Matrix4 rotationMatrix;
  float zoomOffset;

public:
  PanCameraInputHandler(uint id, const Vector3D& position, float zoomOffset = 0, float yaw = 0, float pitch = -45);

  Vector3D getPosition() const { return position; }
  float getPitch() const { return pitch; };
  float getYaw() const { return yaw; };
  void getZoomOffset(float zoomOffset) { this->zoomOffset = zoomOffset; }
  void setPanSpeed(float speed) { panSpeed = speed; }
  void setYawSpeed(float speed) { yawSpeed = speed; }
  void refreshCamera(Camera* camera) const;

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;
  virtual bool onKeyHeld(GameContext* gameContext, uint key) override;

protected:
  void refreshRotationMatrix();
  };
