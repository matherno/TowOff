#pragma once

#include <UISystem/UIPanel.h>
#include <UISystem/UIProgressBar.h>
#include <UISystem/UIButton.h>
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
  TowerPtr activeHUDTower;
  std::shared_ptr<TowerPlacementHandler> placementHandler;

public:
  void initialiseUI(GameContext* context);
  void updateUI(GameContext* context);
  void deselectTowerType();

protected:
  void setupTowerFocusPanel(GameContext* context);
  void setupTowerBuildPanel(GameContext* context);
  void startTowerPlacingMode(GameContext* gameContext, uint towerTypeID);
  void endTowerPlacingMode(GameContext* gameContext);
  };
