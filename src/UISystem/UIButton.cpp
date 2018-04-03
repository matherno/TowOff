//
// Created by matt on 28/01/18.
//

#include "UIButton.h"

#define BUTTON_PRESS_TIME 150

UIButton::UIButton(uint id, bool toggle) : UIPanel(id), toggle(toggle)
  {}

void UIButton::initialise(GameContext* context)
  {
  UIPanel::initialise(context);
  buttonInnerComponent.reset(new UIText(context->getUIManager()->getNextComponentID()));
  addChild(buttonInnerComponent);
  pressTimer.setTimeOut(BUTTON_PRESS_TIME);
  }

void UIButton::refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize)
  {
  buttonInnerComponent->setHorizontalAlignment(Alignment::alignmentCentre);
  buttonInnerComponent->setVerticalAlignment(Alignment::alignmentCentre);
  buttonInnerComponent->setHeightMatchParent(true);
  buttonInnerComponent->setWidthMatchParent(true);
  buttonInnerComponent->setPadding(highlightWidth, highlightWidth);
  buttonInnerComponent->setTexture(buttonTexture);
  buttonInnerComponent->setColour(buttonColour);
  buttonInnerComponent->setText(text);
  buttonInnerComponent->setFontSize(textSize);
  buttonInnerComponent->setFontColour(textColour);
  buttonInnerComponent->setTextCentreAligned(true, true);
  buttonInnerComponent->setMouseClickCallback([this](uint mouseX, uint mouseY)
                                                {
                                                return onButtonClick(mouseX, mouseY);
                                                });
  UIPanel::refresh(context, parentPos, parentSize);
  }

bool UIButton::onButtonClick(uint mouseX, uint mouseY)
  {
  pressed = toggle ? !pressed : true;
  pressTimer.reset();
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
  if (group)
    group->toggleButtons.remove(getID());
  UIPanel::cleanUp(context);
  }

void UIButton::setButtonHighlightColour(const Vector3D& pressedColour, const Vector3D& unpressedColour)
  {
  this->pressedColour = pressedColour;
  this->unpressedColour = unpressedColour;
  setColour(pressed ? pressedColour : unpressedColour);
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

void UIButton::onUpdate(GameContext* context)
  {
  UIPanel::onUpdate(context);
  if (!toggle && pressed && pressTimer.incrementTimer(context->getDeltaTime()))
    {
    pressed = false;
    setButtonHighlightColour(pressedColour, unpressedColour);
    }
  }
