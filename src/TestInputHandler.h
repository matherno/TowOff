#pragma once

#include <GameSystem/GameSystem.h>

class TestInputHandler : public InputHandler
  {
private:
  GameActorPtr actor;

public:
  TestInputHandler(uint id);

  virtual void onDetached(GameContext* gameContext) override;

  virtual void onAttached(GameContext* gameContext) override;

  virtual bool onMousePressed(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;

  virtual bool onMouseHeld(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;

  virtual bool onMouseReleased(GameContext* gameContext, uint button, uint mouseX, uint mouseY) override;

  virtual bool onMouseScroll(GameContext* gameContext, double scrollOffset, uint mouseX, uint mouseY) override;

  virtual bool onMouseMove(GameContext* gameContext, uint mouseX, uint mouseY, uint prevMouseX, uint prevMouseY) override;

  virtual bool onKeyPressed(GameContext* gameContext, uint key) override;

  virtual bool onKeyHeld(GameContext* gameContext, uint key) override;

  virtual bool onKeyReleased(GameContext* gameContext, uint key) override;
  };
