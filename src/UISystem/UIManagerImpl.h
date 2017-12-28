#pragma once

#include "UISystem.h"

/*
*   
*/

class UIManagerImpl : public UIManager
  {
private:
  uint nextComponentID = 1;
  UIComponentPtr rootComponent;

public:
  virtual void addComponent(UIComponentPtr component) override;
  virtual void removeComponent(uint id) override;
  virtual uint getNextComponentID() override;
  virtual bool mouseClick(GameContext* context, uint mouseX, uint mouseY) override;
  virtual bool initialise(GameContext* context) override;
  virtual void update(GameContext* context) override;
  virtual void cleanUp(GameContext* context) override;
  virtual bool hitTest(uint mouseX, uint mouseY) override;
  };
