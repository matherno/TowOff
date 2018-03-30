#pragma once

#include "mathernogl/MathernoGL.h"
#include "GameSystem.h"

typedef mathernogl::InputHandler InputSource;


class InputManagerImpl : public InputManager
  {
private:
  mathernogl::MappedList<InputHandlerPtr> handlers;
  mathernogl::MappedList<InputHandlerPtr> priorityHandlers;
  uint nextHandlerID = 1;
  InputSource inputSource;
  int prevMouseX, prevMouseY;

public:
  virtual void initialise(mathernogl::Window* window) override;
  virtual void cleanUp() override;
  virtual uint getNextHandlerID() override;
  virtual void addHandler(InputHandlerPtr handler) override;
  virtual void addPriorityHandler(InputHandlerPtr handler) override;
  virtual void removeHandler(uint id) override;
  virtual bool containsHandler(uint id) const override;
  virtual void processInput(GameContext* gameContext) override;
  virtual bool isKeyDown(uint key) const override;

  void fireMousePressedEvents(GameContext* gameContext, uint button, uint mouseX, uint mouseY);
  void fireMouseHeldEvents(GameContext* gameContext, uint button, uint mouseX, uint mouseY);
  void fireMouseReleasedEvents(GameContext* gameContext, uint button, uint mouseX, uint mouseY);
  void fireMouseScrollEvents(GameContext* gameContext, double scrollOffset, uint mouseX, uint mouseY);
  void fireMouseMoveEvents(GameContext* gameContext, uint mouseX, uint mouseY, uint prevMouseX, uint prevMouseY);
  void fireKeyPressedEvents(GameContext* gameContext, uint key);
  void fireKeyHeldEvents(GameContext* gameContext, uint key);
  void fireKeyReleasedEvents(GameContext* gameContext, uint key);
  };
