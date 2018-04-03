//
// Created by matt on 27/12/17.
//

#include <GameSystem/InputCodes.h>
#include "UIManagerImpl.h"
#include "UIPanel.h"
#include "UIInputHandler.h"

void UIManagerImpl::addComponent(UIComponentPtr component)
  {
  rootComponent->addChild(component);
  }

void UIManagerImpl::removeComponent(uint id)
  {
  rootComponent->removeChild(id);
  }

bool UIManagerImpl::initialise(GameContext* context)
  {
  rootComponent.reset(new UIPanel(getNextComponentID()));
  rootComponent->setHeightMatchParent(true);
  rootComponent->setWidthMatchParent(true);
  rootComponent->initialise(context);
  rootComponent->refresh(context, Vector2D(0, 0), context->getRenderContext()->getWindow()->getSize());
  rootComponent->setVisible(false);
  UIInputHandler* inputHandler = new UIInputHandler(context->getInputManager()->getNextHandlerID());
  context->addInputHandler(InputHandlerPtr(inputHandler), true);
  return true;
  }

void UIManagerImpl::update(GameContext* context)
  {
  rootComponent->onUpdate(context);
  }

void UIManagerImpl::cleanUp(GameContext* context)
  {
  rootComponent->cleanUp(context);
  }

UIComponentPtr UIManagerImpl::getComponent(uint id, bool recurseChildren)
  {
  return rootComponent->getChild(id, recurseChildren);
  }

uint UIManagerImpl::getNextComponentID()
  {
  return nextComponentID++;
  }

bool UIManagerImpl::mouseClick(GameContext* context, uint mouseX, uint mouseY)
  {
  uint componentClicked = 0;
  if (isModalModeActive())
    componentClicked = (*modalComponents.begin())->mouseClick(context, mouseX, mouseY);
  else
    componentClicked = rootComponent->mouseClick(context, mouseX, mouseY);
  if (componentClicked == 0)
    {
    lossFocus(context);
    return false;
    }

  if (UIComponentPtr newFocusComponent = getComponent(componentClicked, true))
    {
    bool changingFocus = !focusedComponent || focusedComponent->getID() != newFocusComponent->getID();
    if (changingFocus)
      {
      lossFocus(context);
      focusedComponent = newFocusComponent;
      focusedComponent->onGainFocus(context);
      }
    }

  return true;
  }

bool UIManagerImpl::keyPress(GameContext* context, uint key)
  {
  if (key == KEY_ESC && isModalModeActive())
    {
    (*modalComponents.begin())->onEscapePressed(context);
    return true;
    }

  if (focusedComponent)
    return focusedComponent->keyPress(context, key);
  return false;
  }

void UIManagerImpl::lossFocus(GameContext* context)
  {
  if (focusedComponent)
    {
    focusedComponent->onLossFocus(context);
    focusedComponent.reset();
    }
  }

bool UIManagerImpl::hitTest(uint mouseX, uint mouseY)
  {
  return rootComponent->hitTest(mouseX, mouseY, true);
  }

void UIManagerImpl::pushModalComponent(UIComponentPtr modalComponent)
  {
  modalComponents.push_front(modalComponent);
  }

void UIManagerImpl::popModalComponent()
  {
  modalComponents.pop_front();
  }

bool UIManagerImpl::isModalModeActive() const
  {
  return !modalComponents.empty();
  }

