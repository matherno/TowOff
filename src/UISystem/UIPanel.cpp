//
// Created by matt on 27/12/17.
//

#include "UIPanel.h"

UIPanel::UIPanel(uint id) : UIComponent(id)
  {}

void UIPanel::initialise(GameContext* context)
  {
  RenderContext* renderContext = context->getRenderContext();
  renderable.reset(new UIRenderable(renderContext->getNextRenderableID()));
  renderable->initialise(renderContext);
  renderContext->getRenderableSet()->addRenderable(renderable);
  isValid = false;
  }

void UIPanel::onUpdate(GameContext* context)
  {
  removePendingComponents(context);
  addPendingComponents(context);
  refreshPendingComponents(context);
  for (UIComponentPtr& child : *children.getList())
    child->onUpdate(context);
  }

void UIPanel::cleanUp(GameContext* context)
  {
  RenderContext* renderContext = context->getRenderContext();
  renderable->cleanUp(renderContext);
  renderContext->getRenderableSet()->removeRenderable(renderable->getID());
  renderable.reset();
  for (UIComponentPtr& child : *children.getList())
    child->cleanUp(context);
  children.clear();
  isValid = false;
  }

void UIPanel::refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize)
  {
  if (onRefreshCallback)
    onRefreshCallback(parentPos, parentSize);

  if (renderable)
    {
    Vector2D adjustedSize = size;
    Vector2D position = parentPos;

    if (widthMatchParent)
      {
      float padding = std::min(horizPadding, (parentSize.x * 0.5f) - 1.0f);
      adjustedSize.x = parentSize.x - (padding * 2);
      position.x += padding;
      }
    else
      {
      switch (horizAlignment)
        {
        case alignmentCentre:
          position.x += (parentSize.x * 0.5f) - (adjustedSize.x * 0.5);
          break;
        case alignmentEnd:
          position.x += parentSize.x - adjustedSize.x;
          break;
        }
      position.x += offset.x;
      }

    if (heightMatchParent)
      {
      float padding = std::min(vertPadding, (parentSize.y * 0.5f) - 1.0f);
      adjustedSize.y = parentSize.y - (padding * 2);
      position.y += padding;
      }
    else
      {
      switch (vertAlignment)
        {
        case alignmentCentre:
          position.y += (parentSize.y * 0.5f) - (adjustedSize.y * 0.5);
          break;
        case alignmentEnd:
          position.y += parentSize.y - adjustedSize.y;
          break;
        }
      position.y += offset.y;
      }

    adjustedSize.x = std::max(adjustedSize.x, 1.0f);
    adjustedSize.y = std::max(adjustedSize.y, 1.0f);

    renderable->setColour(colour);
    renderable->setTexture(texture, alphaTexture);
    renderable->setVisible(visible);
    renderable->setTextureCoords(texCoordBL, texCoordTR);
    renderable->refresh(position, adjustedSize);
    for (UIComponentPtr& child : *children.getList())
      child->refresh(context, position, adjustedSize);
    currentScreenPos = position;
    currentScreenSize = adjustedSize;
    isValid = true;
    }
  }

bool UIPanel::needsRefresh()
  {
  return !isValid;
  }

void UIPanel::invalidate()
  {
  isValid = false;
  }

void UIPanel::addChild(UIComponentPtr component)
  {
  childrenToAdd.push_back(component);
  }

void UIPanel::removeChild(uint id)
  {
  childrenToRemove.push_back(id);
  }

void UIPanel::refreshPendingComponents(GameContext* context)
  {
  for (UIComponentPtr comp : *children.getList())
    {
    if (comp->needsRefresh())
      comp->refresh(context, currentScreenPos, currentScreenSize);
    }
  }

void UIPanel::addPendingComponents(GameContext* context)
  {
  for (UIComponentPtr comp : childrenToAdd)
    {
    children.add(comp, comp->getID());
    comp->initialise(context);
    }
  childrenToAdd.clear();
  }

void UIPanel::removePendingComponents(GameContext* context)
  {
  for (uint id : childrenToRemove)
    {
    UIComponentPtr comp = children.get(id);
    if (comp)
      {
      children.remove(comp->getID());
      comp->cleanUp(context);
      }
    }
  childrenToRemove.clear();
  }

void UIPanel::setVisible(bool visible, bool recurseChildren)
  {
  this->visible = visible;
  if(renderable)
    renderable->setVisible(visible);
  if (recurseChildren)
    {
    for (UIComponentPtr child : *children.getList())
      child->setVisible(visible, recurseChildren);
    }
  }

bool UIPanel::mouseClick(GameContext* context, uint mouseX, uint mouseY)
  {
  for (UIComponentPtr comp : *children.getList())
    {
    if (comp->mouseClick(context, mouseX, mouseY))
      return true;
    }
  if (mouseClickCallback && hitTest(mouseX, mouseY, false))
    return mouseClickCallback(mouseX, mouseY);
  return false;
  }

bool UIPanel::hitTest(uint mouseX, uint mouseY, bool testChildren)
  {
  if (visible && mouseX >= currentScreenPos.x && mouseX <= currentScreenPos.x + currentScreenSize.x)
    {
    if (mouseY >= currentScreenPos.y && mouseY <= currentScreenPos.y + currentScreenSize.y)
      return true;
    }

  if (testChildren)
    {
    for (UIComponentPtr comp : *children.getList())
      {
      if (comp->hitTest(mouseX, mouseY, true))
        return true;
      }
    }
  return false;
  }

void UIPanel::setPadding(float horizPadding, float vertPadding)
  {
  this->horizPadding = horizPadding;
  this->vertPadding = vertPadding;
  }