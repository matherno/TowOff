//
// Created by matt on 28/12/17.
//

#include <GameSystem/InputCodes.h>
#include "UIInputHandler.h"

UIInputHandler::UIInputHandler(uint id) : InputHandler(id)
  {}

void UIInputHandler::onAttached(GameContext* gameContext)
  {

  }

void UIInputHandler::onDetached(GameContext* gameContext)
  {

  }

bool UIInputHandler::onMousePressed(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  if (gameContext->getUIManager()->hitTest(mouseX, mouseY))
    {
    if (button == MOUSE_LEFT)
      gameContext->getUIManager()->mouseClick(gameContext, mouseX, mouseY);
    return true;
    }
  else
    {
    gameContext->getUIManager()->lossFocus(gameContext);
    return false;
    }
  }

bool UIInputHandler::onMouseHeld(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  return gameContext->getUIManager()->hitTest(mouseX, mouseY);
  }

bool UIInputHandler::onMouseReleased(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  return gameContext->getUIManager()->hitTest(mouseX, mouseY);
  }

bool UIInputHandler::onMouseScroll(GameContext* gameContext, double scrollOffset, uint mouseX, uint mouseY)
  {
  return gameContext->getUIManager()->hitTest(mouseX, mouseY);
  }

bool UIInputHandler::onKeyPressed(GameContext* gameContext, uint key)
  {
  return gameContext->getUIManager()->keyPress(gameContext, key);
  }

bool UIInputHandler::onKeyRepeated(GameContext* gameContext, uint key)
  {
  return gameContext->getUIManager()->keyPress(gameContext, key);
  }
