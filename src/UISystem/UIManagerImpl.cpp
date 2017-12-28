//
// Created by matt on 27/12/17.
//

#include "UIManagerImpl.h"
#include "UIPanel.h"

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