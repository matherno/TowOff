//
// Created by matherno on 23/11/17.
//

#include <GameSystem/InputCodes.h>
#include "TestInputHandler.h"
#include "TestActor.h"

TestInputHandler::TestInputHandler(uint id) : InputHandler(id)
  {}

void TestInputHandler::onAttached(GameContext* gameContext)
  {
  actor.reset(new TestActor(gameContext->getNextActorID()));
  gameContext->addActor(actor);
  }

bool TestInputHandler::onMousePressed(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  mathernogl::logInfo("Mouse pressed " + std::to_string(button));
  return false;
  }

bool TestInputHandler::onMouseHeld(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  mathernogl::logInfo("Mouse held " + std::to_string(button));
  return false;
  }

bool TestInputHandler::onMouseReleased(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  mathernogl::logInfo("Mouse released " + std::to_string(button));
  return false;
  }

bool TestInputHandler::onMouseScroll(GameContext* gameContext, double scrollOffset, uint mouseX, uint mouseY)
  {
  return false;
  }

bool TestInputHandler::onMouseMove(GameContext* gameContext, uint mouseX, uint mouseY, uint prevMouseX, uint prevMouseY)
  {
  return false;
  }

bool TestInputHandler::onKeyPressed(GameContext* gameContext, uint key)
  {
  mathernogl::logInfo("Key " + std::to_string(key) + " pressed");
  return false;
  }

bool TestInputHandler::onKeyHeld(GameContext* gameContext, uint key)
  {
  Vector3D translation(0);
  double speed = 0.1;
  char character = getCharFromKeyCode(key);
  switch(character)
    {
    case 'W':
      translation.set(0, speed, 0);
      break;
    case 'A':
      translation.set(-speed, 0, 0);
      break;
    case 'S':
      translation.set(0, -speed, 0);
      break;
    case 'D':
      translation.set(speed, 0, 0);
      break;
    }
  dynamic_cast<TestActor*>(actor.get())->translate(translation);
  return false;
  }

bool TestInputHandler::onKeyReleased(GameContext* gameContext, uint key)
  {
  mathernogl::logInfo("Key " + std::to_string(key) + " released");
  return false;
  }

void TestInputHandler::onDetached(GameContext* gameContext)
  {

  }

