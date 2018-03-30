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
  UIComponentPtr modalComponent;

public:
  virtual void addComponent(UIComponentPtr component) override;
  virtual void removeComponent(uint id) override;
  virtual UIComponentPtr getComponent(uint id) override;
  virtual uint getNextComponentID() override;
  virtual void enableModalMode(UIComponentPtr modalComponent) override;
  virtual void disableModalMode() override;
  virtual bool isModalModeActive() const override;
  virtual bool mouseClick(GameContext* context, uint mouseX, uint mouseY) override;
  virtual bool initialise(GameContext* context) override;
  virtual void update(GameContext* context) override;
  virtual void cleanUp(GameContext* context) override;
  virtual bool hitTest(uint mouseX, uint mouseY) override;
  };
