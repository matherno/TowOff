#pragma once

#include <GameSystem/GameSystem.h>
#include <RenderSystem/RenderableLines.h>
#include "RangeFieldManager.h"

/*
*   
*/

class TowerPlacementHandler : public InputHandler
  {
private:
  const uint towerTypeID;
  RenderableSetPtr towerHighlight;
  std::shared_ptr<RenderableLines> towerConnectionHighlights;
  std::shared_ptr<RenderableLineStrips> towerRangeHighlight;
  Vector3D towerHighlightPos;
  bool isHighlightPosValid = false;
  CallbackFunc endHandlerCallback;
  Vector3D highlightColourValid = Vector3D(0.1, 0.5, 0.1);
  Vector3D highlightColourInvalid = Vector3D(0.5, 0.1, 0.1);
  Vector3D previewColour = Vector3D(0.5);
  Vector3D relayConnectionColour = Vector3D(0, 0.6, 0.7);
  float previewColourAlpha = 0.5f;

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
  void setupRangeHighlight (GameContext* gameContext);
  bool isHighlightTowerColliding(GameContext* gameContext, const Vector3D& towerHighlightPosition) const;
  };
