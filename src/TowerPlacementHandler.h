#pragma once

#include <src/GameSystem/GameSystem.h>
#include <src/RenderSystem/RenderableLines.h>

/*
*   
*/

class TowerPlacementHandler : public InputHandler
  {
private:
  const uint towerTypeID;
  RenderableSetPtr towerHighlight;
  std::shared_ptr<RenderableLines> towerConnectionHighlights;
  Vector3D towerHighlightPos;
  bool isPositionValid = false;
  CallbackFunc endHandlerCallback;
  Vector3D highlightColourValid = Vector3D(0.1, 0.5, 0.1);
  Vector3D highlightColourInvalid = Vector3D(0.5, 0.1, 0.1);
  Vector3D connectionColour = Vector3D(0.5);
  float connectionColourAlpha = 0.5f;

public:
  TowerPlacementHandler(uint id, uint towerTypeID);
  void setEndHandlerCallback(CallbackFunc func){ endHandlerCallback = func; }

private:
  virtual void onAttached(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;
  virtual bool onMousePressed(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;
  virtual bool onMouseHeld(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;
  virtual bool onMouseReleased(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;
  virtual bool onMouseMove(GameContext* gameContext, uint mouseX, uint mouseY, uint prevMouseX, uint prevMouseY) override;

  void setupHighlightColour ();
  void setupConnectionHighlights (GameContext* gameContext);
  };
