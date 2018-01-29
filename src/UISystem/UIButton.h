#pragma once

#include "UIPanel.h"

/*
*   
*/

class UIToggleButtonGroup;
typedef std::shared_ptr<UIToggleButtonGroup> UIToggleButtonGroupPtr;

class UIButton : public UIPanel
  {
private:
  bool pressed = false;
  bool toggle = false;
  float highlightWidth = 1;
  TexturePtr buttonTexture;
  Vector3D pressedColour = Vector3D(0, 0, 1);
  Vector3D unpressedColour = Vector3D(0.3);
  std::shared_ptr<UIPanel> buttonImageComponent;
  OnMouseClickCallback buttonClickCallback;
  UIToggleButtonGroupPtr group;

public:
  UIButton(uint id, bool toggle);
  void setHighlightWidth(float width) { highlightWidth = width; }
  void setButtonTexture(TexturePtr button){ buttonTexture = button; }
  void setButtonHighlightColour(const Vector3D& pressedColour, const Vector3D& unpressedColour = Vector3D(0.3));
  bool isToggledDown() const { return toggle && pressed; }
  void setGroup(UIToggleButtonGroupPtr group);

  virtual void initialise(GameContext* context) override;
  virtual void setMouseClickCallback(OnMouseClickCallback func) override;
  virtual void refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize) override;
  virtual void cleanUp(GameContext* context) override;

protected:
  bool onButtonClick(uint mouseX, uint mouseY);
  void onUnpress(uint mouseX, uint mouseY);
  void updateGroup(uint mouseX, uint mouseY);
  };

class UIToggleButtonGroup
  {
private:
  friend class UIButton;
  mathernogl::MappedList<UIButton*> toggleButtons;
  };
