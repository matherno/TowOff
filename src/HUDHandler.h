#pragma once

#include <UISystem/UIPanel.h>
#include <src/UISystem/UIProgressBar.h>
#include <src/UISystem/UIButton.h>
#include "Tower.h"

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
  int selectedTowerType = -1;
  std::weak_ptr<Tower> focusedTower;
  bool newFocusedTower = false;

public:
  void initialiseUI(GameContext* context);
  void updateUI(GameContext* context);
  bool isTowerTypeSelected() const { return selectedTowerType >= 0; }
  uint getTowerTypeSelected() const { return isTowerTypeSelected() ? (uint)selectedTowerType : 0; }
  void deselectTowerType();
  void setTowerFocused(TowerPtr tower);

protected:
  void onSelectedTowerType(uint towerType);
  void onDeselectedTowerType();
  void setupTowerFocusPanel(GameContext* context);
  void setupTowerBuildPanel(GameContext* context);
  };
