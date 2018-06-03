//
// Created by matt on 16/02/18.
//

#include <RenderSystem/RenderableMesh.h>
#include <RenderSystem/RenderableSetImpl.h>
#include <GameSystem/InputCodes.h>
#include "TowerPlacementHandler.h"
#include "TowerFactory.h"
#include "TOGameContext.h"
#include "PhasingMesh.h"
#include "Deposit.h"

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
    PhasingMesh* mesh = new PhasingMesh(renderContext->getNextRenderableID(), false);
    mesh->setMeshStorage(renderContext->getSharedMeshStorage(type->baseMeshFilePath));
    mesh->initialise(renderContext);
    towerHighlight->addRenderable(RenderablePtr(mesh));
    }

  if (!type->turretMeshFilePath.empty())
    {
    PhasingMesh* mesh = new PhasingMesh(renderContext->getNextRenderableID(), false);
    mesh->setMeshStorage(renderContext->getSharedMeshStorage(type->turretMeshFilePath));
    mesh->initialise(renderContext);
    towerHighlight->addRenderable(RenderablePtr(mesh));
    }

  towerConnectionHighlights.reset(new RenderableLines(renderContext->getNextRenderableID()));
  towerConnectionHighlights->initialise(renderContext);
  renderContext->getRenderableSet()->addRenderable(towerConnectionHighlights);

  towerRangeHighlight.reset(new RenderableLineStrips(renderContext->getNextRenderableID()));
  towerRangeHighlight->initialise(renderContext);
  towerRangeHighlight->setLineWidth(3);
  renderContext->getRenderableSet()->addRenderable(towerRangeHighlight);

  towerHighlightPos.set(0, 0, 0);
  setupHighlightColour();
  setupRangeHighlight(gameContext);
  TOGameContext::cast(gameContext)->displayAllRelayRanges();
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

  TOGameContext::cast(gameContext)->hideAllRangeFields();
  renderContext->getRenderableSet()->removeRenderable(towerConnectionHighlights->getID());
  towerConnectionHighlights->cleanUp(renderContext);
  renderContext->getRenderableSet()->removeRenderable(towerRangeHighlight->getID());
  towerRangeHighlight->cleanUp(renderContext);
  }

bool TowerPlacementHandler::onMouseReleased(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  TOGameContext* toGameContext = TOGameContext::cast(gameContext);
  if (button == MOUSE_LEFT)
    {
    if (isHighlightPosValid)
      {
      const TowerType* towerType = TowerFactory::getTowerType(towerTypeID);
      TowerPtr tower = toGameContext->createTower(towerTypeID, towerHighlightPos, true);

      if (!gameContext->getInputManager()->isKeyDown(KEY_LCTRL))
        {
        if (endHandlerCallback)
          endHandlerCallback();
        }
      else
        {
        //  placing another tower
        toGameContext->displayAllRelayRanges();
        }
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

  bool isNewPosValid = isLand;
  isNewPosValid = isNewPosValid && FogOfWarHandler::isVisibleAtPosition(gameContext, towerHighlightPos);
  isNewPosValid = isNewPosValid && !isHighlightTowerColliding(gameContext, towerHighlightPos);
  if (isHighlightPosValid != isNewPosValid)
    {
    isHighlightPosValid = isNewPosValid;
    setupHighlightColour();
    }
  setupConnectionHighlights(gameContext);
  setupRangeHighlight(gameContext);
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
  Vector3D colour = isHighlightPosValid ? highlightColourValid : highlightColourInvalid;
  towerHighlight->forEachChild([colour](RenderablePtr child)
                                 {
                                 RenderableMesh* mesh = dynamic_cast<RenderableMesh*>(child.get());
                                 if (mesh)
                                   mesh->setDrawStyleSingleColour(colour);
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

    Vector3D colour = previewColour;
    if (TowerFactory::canTowerRelayEnergy(tower->getFunction()) && TowerFactory::canTowerRelayEnergy(towerHighlightFunction))
        colour = relayConnectionColour;
    towerConnectionHighlights->addLine(towerHighlightConnectPos, tower->getConnectPosition(), colour, previewColourAlpha);
    }

  towerConnectionHighlights->enableRebuild();
  towerConnectionHighlights->rebuildBuffer();
  }

void TowerPlacementHandler::setupRangeHighlight(GameContext* gameContext)
  {
  towerRangeHighlight->disableRebuild();
  towerRangeHighlight->clearLineStrips();

  towerRangeHighlight->startLineStrip(previewColour, previewColourAlpha);
  float radius = TowerFactory::getTowerRange(towerTypeID);
  if (radius > 0)
    towerRangeHighlight->addXZPlaneCircle(towerHighlightPos + Vector3D(0, 0.02, 0), radius, 60);
  towerRangeHighlight->finishLineStrip();

  towerRangeHighlight->enableRebuild();
  towerRangeHighlight->rebuildBuffer();
  }

bool TowerPlacementHandler::isHighlightTowerColliding(GameContext* gameContext, const Vector3D& towerHighlightPosition) const
  {
  const float towerHighlightHitRadius = TowerFactory::getTowerType(towerTypeID)->hitRadius;
  for (const TowerPtr tower : *TOGameContext::cast(gameContext)->getTowers()->getList())
    {
    const float minDistance = tower->getHitRadius() + towerHighlightHitRadius;
    if (towerHighlightPosition.distanceToPoint(tower->getPosition()) <= minDistance)
      return true;    // highlight tower collides with an existing tower
    }

  for (const DepositPtr deposit : *TOGameContext::cast(gameContext)->getDepositList()->getList())
    {
    const float minDistance = deposit->getHitRadius() + towerHighlightHitRadius;
    if (towerHighlightPosition.distanceToPoint(deposit->getPosition()) <= minDistance)
      return true;    // highlight tower collides with a resource deposit
    }
  return false;
  }
