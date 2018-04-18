#pragma once

#include <GameSystem/GameSystem.h>

/*
*   
*/

class TOInputHandler : public InputHandler
  {
private:
  Vector3D focalPosition;
  float panSpeed = 50;
  float yawSpeed = 90;
  float zoomSpeed = 8;
  float mousePanSpeed = 50;
  float minZoom = 30;
  float maxZoom = 100;
  float rotation;
  float pitch;
  Matrix4 rotationMatrix;
  float zoomOffset;
  bool paused = false;
  uint activePlayer = 0;

public:
  TOInputHandler(uint id, const Vector3D& focalPosition, float zoomOffset = 0, float rotation = 0, float pitch = -45);

  Vector3D getFocalPosition() const { return focalPosition; }
  float getPitch() const { return pitch; };
  float getRotation() const { return rotation; };
  float getZoomOffset() { return zoomOffset; }
  void setPanSpeed(float speed) { panSpeed = speed; }
  void setYawSpeed(float speed) { yawSpeed = speed; }
  void setZoomOffset(float zoomOffset) { this->zoomOffset = zoomOffset; }
  void setZoomSpeed(float speed) { zoomSpeed = speed; }
  void setZoomLimits(float min, float max) { minZoom = min; maxZoom = max; }
  void refreshCamera(Camera* camera) const;
  void setActivePlayer(uint player) { activePlayer = player; }
  uint getActivePlayer() const { return activePlayer; }

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual bool onMousePressed(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;
  virtual bool onKeyPressed(GameContext* gameContext, uint key) override;
  virtual void onDetached(GameContext* gameContext) override;
  virtual bool onKeyHeld(GameContext* gameContext, uint key) override;
  virtual bool onMouseScroll(GameContext* gameContext, double scrollOffset, uint mouseX, uint mouseY) override;
  virtual bool onMouseHeld(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;
  virtual bool onMouseReleased(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;

protected:
  void refreshRotationMatrix();
  void performMouseCameraMovement(GameContext* gameContext);
  };
