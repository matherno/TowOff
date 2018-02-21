//
// Created by matt on 16/02/18.
//

#include <src/RenderSystem/RenderableMesh.h>
#include <src/RenderSystem/RenderableSetImpl.h>
#include <src/GameSystem/InputCodes.h>
#include "TowerPlacementHandler.h"
#include "TowerFactory.h"
#include "TOGameContext.h"
#include "PhasingMesh.h"

TowerPlacementHandler::TowerPlacementHandler(uint id, uint towerTypeID) : InputHandler(id), towerTypeID(towerTypeID)
  {}

void TowerPlacementHandler::onAttached(GameContext* gameContext)
  {
  const TowerType* type = TowerFactory::getTowerType(towerTypeID);
  RenderContext* renderContext = gameContext->getRenderContext();
  towerHighlight = renderContext->createRenderableSet();
  renderContext->getRenderableSet()->addRenderable(towerHighlight);

  if (!type->baseMeshFilePath.empty())
    {
    PhasingMesh* mesh = new PhasingMesh(renderContext->getNextRenderableID());
    mesh->setMeshStorage(renderContext->createMeshStorage(type->baseMeshFilePath));
    mesh->initialise(renderContext);
    towerHighlight->addRenderable(RenderablePtr(mesh));
    }

  if (!type->turretMeshFilePath.empty())
    {
    PhasingMesh* mesh = new PhasingMesh(renderContext->getNextRenderableID());
    mesh->setMeshStorage(renderContext->createMeshStorage(type->turretMeshFilePath));
    mesh->initialise(renderContext);
    towerHighlight->addRenderable(RenderablePtr(mesh));
    }

  towerConnectionHighlights.reset(new RenderableLines(renderContext->getNextRenderableID()));
  towerConnectionHighlights->initialise(renderContext);
  renderContext->getRenderableSet()->addRenderable(towerConnectionHighlights);

  towerHighlightPos.set(0, 0, 0);
  setupHighlightColour();
  }

void TowerPlacementHandler::onDetached(GameContext* gameContext)
  {
  RenderContext* renderContext = gameContext->getRenderContext();
  towerHighlight->forEachChild([renderContext](RenderablePtr child)
    {
    renderContext->getRenderableSet()->removeRenderable(child->getID());
    child->cleanUp(renderContext);
    });
  renderContext->getRenderableSet()->removeRenderable(towerHighlight->getID());
  towerHighlight->cleanUp(renderContext);

  renderContext->getRenderableSet()->removeRenderable(towerConnectionHighlights->getID());
  towerConnectionHighlights->cleanUp(renderContext);
  }

bool TowerPlacementHandler::onMouseReleased(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  TOGameContext* toGameContext = TOGameContext::cast(gameContext);
  if (button == MOUSE_LEFT)
    {
    if (isPositionValid)
      {
      const TowerType* towerType = TowerFactory::getTowerType(towerTypeID);
      bool needsConstruction = towerType->name != "Home Base" && towerType->name != "Enemy Tower";
      TowerPtr tower = toGameContext->createTower(towerTypeID, towerHighlightPos, needsConstruction);
      if (towerType->name == "Enemy Tower")
        tower->setPlayerNum(2);
      else
        tower->setPlayerNum(1);

      if (!gameContext->getInputManager()->isKeyDown(KEY_LCTRL) && endHandlerCallback)
        endHandlerCallback();
      }
    }
  else if (button == MOUSE_RIGHT)
    endHandlerCallback();
  return true;
  }

bool TowerPlacementHandler::onMouseMove(GameContext* gameContext, uint mouseX, uint mouseY, uint prevMouseX, uint prevMouseY)
  {
  TOGameContext* toGameContext = TOGameContext::cast(gameContext);
  bool isLand = false;
  Vector3D terrainPos = toGameContext->terrainHitTest(mouseX, mouseY, &isLand);
  towerHighlight->getTransform()->translate(terrainPos - towerHighlightPos);
  towerHighlightPos = terrainPos;
  if (isPositionValid != isLand)
    {
    isPositionValid = isLand;
    setupHighlightColour();
    }
  setupConnectionHighlights(gameContext);
  return true;
  }

bool TowerPlacementHandler::onMousePressed(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  return true;
  }

bool TowerPlacementHandler::onMouseHeld(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  return true;
  }

void TowerPlacementHandler::setupHighlightColour()
  {
  Vector3D colour = isPositionValid ? highlightColourValid : highlightColourInvalid;
  towerHighlight->forEachChild([colour](RenderablePtr child)
                                 {
                                 RenderableMesh* mesh = dynamic_cast<RenderableMesh*>(child.get());
                                 if (mesh)
                                   mesh->setColour(colour);
                                 });
  }

void TowerPlacementHandler::setupConnectionHighlights(GameContext* gameContext)
  {
  TOGameContext* toGameContext = TOGameContext::cast(gameContext);
  Vector3D towerHighlightConnectPos = towerHighlightPos + TowerFactory::getTowerType(towerTypeID)->connectOffset;
  float towerHighlightConnectRadius = TowerFactory::getTowerType(towerTypeID)->connectRadius;
  Tower::TowerFunction towerHighlightFunction = TowerFactory::getTowerFunction(towerTypeID);

  towerConnectionHighlights->disableRebuild();
  towerConnectionHighlights->clearLines();

  for (TowerPtr tower : *toGameContext->getTowers()->getList())
    {
    if (!ConnectionManager::areTowersInRange(tower->getPosition(), tower->getConnectRadius(), towerHighlightPos, towerHighlightConnectRadius))
      continue;
    if (!ConnectionManager::areTowerFunctionsCompatible(tower->getFunction(), towerHighlightFunction))
      continue;
    towerConnectionHighlights->addLine(towerHighlightConnectPos, tower->getConnectPosition(), connectionColour, connectionColourAlpha);
    }

  towerConnectionHighlights->enableRebuild();
  towerConnectionHighlights->rebuildBuffer();
  }
