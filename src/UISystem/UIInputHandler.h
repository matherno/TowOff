#pragma once

#include <GameSystem/GameSystem.h>

/*
*   
*/

class UIInputHandler : public InputHandler
  {
public:
  UIInputHandler(uint id);

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;
  virtual bool onMousePressed(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;
  virtual bool onMouseHeld(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;
  virtual bool onMouseReleased(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;
  virtual bool onMouseScroll(GameContext* gameContext, double scrollOffset, uint mouseX, uint mouseY) override;
  };
