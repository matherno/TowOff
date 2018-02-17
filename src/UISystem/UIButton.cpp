//
// Created by matt on 28/01/18.
//

#include "UIButton.h"

UIButton::UIButton(uint id, bool toggle) : UIPanel(id), toggle(toggle)
  {}

void UIButton::initialise(GameContext* context)
  {
  UIPanel::initialise(context);
  buttonImageComponent.reset(new UIPanel(context->getUIManager()->getNextComponentID()));
  addChild(buttonImageComponent);
  }

void UIButton::refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize)
  {
  //todo implement non toggle button
  buttonImageComponent->setHorizontalAlignment(Alignment::alignmentCentre);
  buttonImageComponent->setVerticalAlignment(Alignment::alignmentCentre);
  buttonImageComponent->setHeightMatchParent(true);
  buttonImageComponent->setWidthMatchParent(true);
  buttonImageComponent->setPadding(highlightWidth, highlightWidth);
  buttonImageComponent->setBackgroundTexture(buttonTexture);
  buttonImageComponent->setMouseClickCallback([this](uint mouseX, uint mouseY)
                                                {
                                                return onButtonClick(mouseX, mouseY);
                                                });
  UIPanel::refresh(context, parentPos, parentSize);
  }

bool UIButton::onButtonClick(uint mouseX, uint mouseY)
  {
  pressed = !pressed;
  setButtonHighlightColour(pressedColour, unpressedColour);
  if (pressed)
    updateGroup(mouseX, mouseY);
  if ((pressed || toggle) && buttonClickCallback)
    return buttonClickCallback(mouseX, mouseY);
  return true;
  }

void UIButton::onUnpress(uint mouseX, uint mouseY)
  {
  pressed = false;
  setButtonHighlightColour(pressedColour, unpressedColour);
  if (toggle && buttonClickCallback)
    buttonClickCallback(mouseX, mouseY);
  }

void UIButton::onForceUnpress()
  {
  pressed = false;
  setButtonHighlightColour(pressedColour, unpressedColour);
  }

void UIButton::updateGroup(uint mouseX, uint mouseY)
  {
  if (group)
    {
    for (UIButton* button : *group->toggleButtons.getList())
      {
      if (button->getID() != getID())
        button->onForceUnpress();
      }
    }
  }

void UIButton::cleanUp(GameContext* context)
  {
  group->toggleButtons.remove(getID());
  UIPanel::cleanUp(context);
  }

void UIButton::setButtonHighlightColour(const Vector3D& pressedColour, const Vector3D& unpressedColour)
  {
  this->pressedColour = pressedColour;
  this->unpressedColour = unpressedColour;
  setBackgroundColour(pressed ? pressedColour : unpressedColour);
  invalidate();
  }

void UIButton::setMouseClickCallback(OnMouseClickCallback func)
  {
  buttonClickCallback = func;
  }

void UIButton::setGroup(UIToggleButtonGroupPtr group)
  {
  ASSERT(toggle, "Must be toggle button to be added to a toggle button group!");
  this->group = group;
  group->toggleButtons.add(this, getID());
  }
