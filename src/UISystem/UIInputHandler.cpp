//
// Created by matt on 28/12/17.
//

#include <src/GameSystem/InputCodes.h>
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
  if (button == MOUSE_LEFT && gameContext->getUIManager()->hitTest(mouseX, mouseY))
    {
    gameContext->getUIManager()->mouseClick(gameContext, mouseX, mouseY);
    return true;
    }
  return false;
  }
