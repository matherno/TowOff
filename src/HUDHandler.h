#pragma once

#include <UISystem/UIPanel.h>
#include <UISystem/UIProgressBar.h>
#include <UISystem/UIButton.h>
#include <UISystem/UIText.h>
#include "Tower.h"
#include "TowerPlacementHandler.h"

/*
*   
*/

class TowerFocusPanel : public UIPanel
  {
private:
  std::shared_ptr<UIPanel> icon;
  std::shared_ptr<UIProgressBar> healthBar;
  std::shared_ptr<UIProgressBar> energyBar;
  std::shared_ptr<UIText> nameText;
  std::shared_ptr<UIText> healthText;
  std::shared_ptr<UIText> energyText;
  TowerPtr activeFocusTower;

public:
  TowerFocusPanel(uint id) : UIPanel(id) {}
  void updateTowerInfo(GameContext* context);

private:
  virtual void initialise(GameContext* context) override;
  };

class TODebugPanel : public UIPanel
  {
private:
  std::shared_ptr<UIText> textComponent;

public:
  TODebugPanel(uint id) : UIPanel(id) {}
  void updateDebugInfo(GameContext* context);
private:
  virtual void initialise(GameContext* context) override;
  };

class HUDHandler
  {
private:
  std::shared_ptr<UIPanel> mainUIPanel;
  std::shared_ptr<TowerFocusPanel> towerFocusPanel;
  std::shared_ptr<TODebugPanel> debugPanel;
  UIToggleButtonGroupPtr towerButtonGroup;
  std::shared_ptr<TowerPlacementHandler> placementHandler;

public:
  void initialiseUI(GameContext* context);
  void updateUI(GameContext* context);
  void cleanUp(GameContext* context);
  void deselectTowerType();
  void toggleDebugPanel();

protected:
  void setupTowerFocusPanel(GameContext* context);
  void setupTowerBuildPanel(GameContext* context);
  void setupDebugPanel(GameContext* context);
  void startTowerPlacingMode(GameContext* gameContext, uint towerTypeID);
  void endTowerPlacingMode(GameContext* gameContext);
  };
