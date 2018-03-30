#pragma once

#include <GameSystem/Timer.h>
#include "UIPanel.h"
#include "UIText.h"

/*
*   
*/

class UIToggleButtonGroup;
typedef std::shared_ptr<UIToggleButtonGroup> UIToggleButtonGroupPtr;

class UIButton : public UIPanel
  {
private:
  friend class UIToggleButtonGroup;
  bool pressed = false;
  bool toggle = false;
  float highlightWidth = 4;
  TexturePtr buttonTexture;
  Vector3D buttonColour;
  Vector3D pressedColour = Vector3D(0, 0, 1);
  Vector3D unpressedColour = Vector3D(0.3);
  std::shared_ptr<UIText> buttonInnerComponent;
  OnMouseClickCallback buttonClickCallback;
  UIToggleButtonGroupPtr group;
  Timer pressTimer;
  string text;
  Vector3D textColour;
  uint textSize;

public:
  UIButton(uint id, bool toggle);
  void setHighlightWidth(float width) { highlightWidth = width; }
  void setButtonTexture(TexturePtr button){ buttonTexture = button; }
  void setButtonColour(const Vector3D& colour){ buttonColour = colour; buttonTexture = nullptr; }
  void setButtonText(string text, const Vector3D& colour, uint size){ this->text = text; textColour = colour; textSize = size; }
  void setButtonHighlightColour(const Vector3D& pressedColour, const Vector3D& unpressedColour = Vector3D(0.3));
  bool isToggledDown() const { return toggle && pressed; }
  void setGroup(UIToggleButtonGroupPtr group);

  virtual void initialise(GameContext* context) override;
  virtual void onUpdate(GameContext* context) override;
  virtual void setMouseClickCallback(OnMouseClickCallback func) override;
  virtual void refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize) override;
  virtual void cleanUp(GameContext* context) override;

protected:
  bool onButtonClick(uint mouseX, uint mouseY);
  void onUnpress(uint mouseX, uint mouseY);
  void onForceUnpress();
  void updateGroup(uint mouseX, uint mouseY);
  };

class UIToggleButtonGroup
  {
private:
  friend class UIButton;
  mathernogl::MappedList<UIButton*> toggleButtons;
public:
  void forceDeselectAll()
    {
    for (UIButton* button : *toggleButtons.getList())
      button->onForceUnpress();
    }
  };
