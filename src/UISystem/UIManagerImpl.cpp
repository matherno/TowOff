//
// Created by matt on 27/12/17.
//

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
  context->addInputHandler(InputHandlerPtr(inputHandler));
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

uint UIManagerImpl::getNextComponentID()
  {
  return nextComponentID++;
  }

bool UIManagerImpl::mouseClick(GameContext* context, uint mouseX, uint mouseY)
  {
  return rootComponent->mouseClick(context, mouseX, mouseY);
  }

bool UIManagerImpl::hitTest(uint mouseX, uint mouseY)
  {
  return rootComponent->hitTest(mouseX, mouseY, true);
  }
