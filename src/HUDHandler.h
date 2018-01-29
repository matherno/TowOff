#pragma once

#include <UISystem/UIPanel.h>

/*
*   
*/

class HUDHandler
  {
private:
  std::shared_ptr<UIPanel> mainUIPanel;
  int selectedTowerType = -1;

public:
  void initialiseUI(GameContext* context);
  bool isTowerTypeSelected() const { return selectedTowerType >= 0; }
  uint getTowerTypeSelected() const { return isTowerTypeSelected() ? (uint)selectedTowerType : 0; }

protected:
  void onSelectedTowerType(uint towerType);
  void onDeselectedTowerType();
  };
