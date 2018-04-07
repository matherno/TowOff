#pragma once

#include <GameSystem/GameSystem.h>
#include <RenderSystem/RenderableLineBoxes.h>
#include "Tower.h"

/*
*   
*/

class TowerSelectionManager : public GameActor
  {
private:
  mathernogl::MappedList<TowerPtr> selectedTowers;
  std::shared_ptr<RenderableLineBoxes> selectionBoxRenderables;
  std::map<uint, uint> towerSelectionBoxesMap;
  Vector3D selectionBoxColour = Vector3D(0.7);
  float selectionBoxAlpha = 0.75f;
  float lineWidth = 1;

public:
  TowerSelectionManager(uint id);

  //  returns true if at least one tower was selected
  bool onWorldClick(GameContext* gameContext, uint mouseX, uint mouseY, bool isCtrlClick);

  void deselectAll(GameContext* gameContext);
  void deselectTower(GameContext* gameContext, uint id);
  void selectTower(GameContext* gameContext, TowerPtr tower);
  bool isTowerSelected(uint towerID) const;
  TowerPtr getFirstSelectedTower();

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

protected:
  void expandBoundingBox(BoundingBoxPtr box);
  };
