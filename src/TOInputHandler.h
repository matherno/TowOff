#pragma once

#include <GameSystem/GameSystem.h>

/*
*   
*/

class TOInputHandler : public InputHandler
  {
private:
  Vector3D position;
  float panSpeed = 50;
  float yawSpeed = 90;
  float zoomSpeed = 8;
  float minZoom = -60;
  float maxZoom = 50;
  float yaw;
  float pitch;
  Matrix4 rotationMatrix;
  float zoomOffset;
  bool paused = false;
  uint activePlayer = 0;

public:
  TOInputHandler(uint id, const Vector3D& position, float zoomOffset = 0, float yaw = 0, float pitch = -45);

  Vector3D getPosition() const { return position; }
  float getPitch() const { return pitch; };
  float getYaw() const { return yaw; };
  void getZoomOffset(float zoomOffset) { this->zoomOffset = zoomOffset; }
  void setPanSpeed(float speed) { panSpeed = speed; }
  void setYawSpeed(float speed) { yawSpeed = speed; }
  void setZoomSpeed(float speed) { zoomSpeed = speed; }
  void setZoomLimits(float min, float max) { minZoom = min; maxZoom = max; }
  void refreshCamera(Camera* camera) const;
  void setActivePlayer(uint player) { activePlayer = player; }
  uint getActivePlayer() const { return activePlayer; }
  void endTowerPlacingMode(GameContext* gameContext);

  virtual void onAttached(GameContext* gameContext) override;
  virtual bool onMousePressed(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;
  virtual bool onKeyPressed(GameContext* gameContext, uint key) override;
  virtual void onDetached(GameContext* gameContext) override;
  virtual bool onKeyHeld(GameContext* gameContext, uint key) override;
  virtual bool onMouseScroll(GameContext* gameContext, double scrollOffset, uint mouseX, uint mouseY) override;

protected:
  void refreshRotationMatrix();
  };
