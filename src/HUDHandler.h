#pragma once

#include <UISystem/UIPanel.h>
#include <src/UISystem/UIProgressBar.h>
#include <src/UISystem/UIButton.h>
#include "Tower.h"
#include "TowerPlacementHandler.h"

/*
*   
*/

class HUDHandler
  {
private:
  std::shared_ptr<UIPanel> mainUIPanel;
  std::shared_ptr<UIPanel> towerFocusPanel;
  std::shared_ptr<UIPanel> towerIcon;
  std::shared_ptr<UIProgressBar> towerHealthBar;
  std::shared_ptr<UIProgressBar> towerEnergyBar;
  UIToggleButtonGroupPtr towerButtonGroup;
  std::weak_ptr<Tower> focusedTower;
  bool newFocusedTower = false;
  std::shared_ptr<TowerPlacementHandler> placementHandler;

public:
  void initialiseUI(GameContext* context);
  void updateUI(GameContext* context);
  void deselectTowerType();
  void setTowerFocused(TowerPtr tower);

protected:
  void setupTowerFocusPanel(GameContext* context);
  void setupTowerBuildPanel(GameContext* context);
  void startTowerPlacingMode(GameContext* gameContext, uint towerTypeID);
  void endTowerPlacingMode(GameContext* gameContext);
  };
