//
// Created by matt on 7/04/18.
//

#include "TowerSelectionManager.h"
#include "TOGameContext.h"

#define SELECTION_BOX_EXPANSION_FACTOR 1.2

TowerSelectionManager::TowerSelectionManager(uint id) : GameActor(id)
  {}

void TowerSelectionManager::onAttached(GameContext* gameContext)
  {
  selectionBoxRenderables.reset(new RenderableLineBoxes(gameContext->getRenderContext()->getNextRenderableID()));
  selectionBoxRenderables->setLineColour(selectionBoxColour);
  selectionBoxRenderables->setAlpha(selectionBoxAlpha);
  selectionBoxRenderables->setLineWidth(lineWidth);
  selectionBoxRenderables->initialise(gameContext->getRenderContext());
  gameContext->getRenderContext()->getRenderableSet()->addRenderable(selectionBoxRenderables);
  }

void TowerSelectionManager::onUpdate(GameContext* gameContext)
  {

  }

void TowerSelectionManager::onDetached(GameContext* gameContext)
  {
  selectionBoxRenderables->cleanUp(gameContext->getRenderContext());
  gameContext->getRenderContext()->getRenderableSet()->removeRenderable(selectionBoxRenderables->getID());
  deselectAll(gameContext);
  }

bool TowerSelectionManager::onWorldClick(GameContext* gameContext, uint mouseX, uint mouseY, bool isCtrlClick)
  {
  if (!isCtrlClick)
    deselectAll(gameContext);

  TOGameContext* toGameContext = TOGameContext::cast(gameContext);
  if (gameContext->getBoundingBoxManager()->boundingBoxPicked())
    {
    uint pickedTowerID = (uint)gameContext->getBoundingBoxManager()->getPickedBoundingBoxMeta();
    if (!isTowerSelected(pickedTowerID))
      {
      if (TowerPtr pickedTower = toGameContext->getTower(pickedTowerID))
        selectTower(gameContext, pickedTower);
      }
    else if (isCtrlClick)
      {
      deselectTower(gameContext, pickedTowerID);
      }
    return true;
    }
  return false;
  }

void TowerSelectionManager::deselectAll(GameContext* gameContext)
  {
  TOGameContext::cast(gameContext)->hideAllRangeFields();
  towerSelectionBoxesMap.clear();
  selectionBoxRenderables->clearBoxes();
  selectedTowers.clear();
  }

void TowerSelectionManager::deselectTower(GameContext* gameContext, uint towerID)
  {
  // remove range field
  TOGameContext::cast(gameContext)->hideTowerRangeField(towerID);
  selectedTowers.remove(towerID);

  // remove tower selection box
  if (towerSelectionBoxesMap.count(towerID) > 0)
    {
    selectionBoxRenderables->removeBox(towerSelectionBoxesMap[towerID]);
    towerSelectionBoxesMap.erase(towerID);
    }
  }

void TowerSelectionManager::selectTower(GameContext* gameContext, TowerPtr tower)
  {
  TOGameContext::cast(gameContext)->displayTowerRangeField(tower.get());
  selectedTowers.add(tower, tower->getID());

  // create selection box if one doesn't already exist for this tower
  if (towerSelectionBoxesMap.count(tower->getID()) == 0)
    {
    BoundingBoxPtr boundingBox = TOGameContext::cast(gameContext)->getTowerCombinedBoundingBox(tower->getID());
    expandBoundingBox(boundingBox);
    uint boxID = selectionBoxRenderables->addBox(boundingBox->getLowerBound(), boundingBox->getUpperBound());
    towerSelectionBoxesMap[tower->getID()] = boxID;
    }
  }

bool TowerSelectionManager::isTowerSelected(uint towerID) const
  {
  return selectedTowers.contains(towerID);
  }

TowerPtr TowerSelectionManager::getFirstSelectedTower()
  {
  if (selectedTowers.count() > 0)
    return *selectedTowers.getList()->begin();
  return nullptr;
  }

void TowerSelectionManager::expandBoundingBox(BoundingBoxPtr box)
  {
  Vector3D expansion = box->getSize() * (SELECTION_BOX_EXPANSION_FACTOR - 1.0) * 0.5;
  Vector3D min = box->getLowerBound() - expansion;
  Vector3D max = box->getUpperBound() + expansion;
  box->setBounds(min, max);
  }
