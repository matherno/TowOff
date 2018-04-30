//
// Created by matt on 20/02/18.
//

#include "UnderConstructTower.h"
#include "PhasingMesh.h"
#include "TOGameContext.h"
#include "TowerFactory.h"


UnderConstructTower::UnderConstructTower(uint id, uint towerType) : Tower(id, towerType, nullptr)
  {}

void UnderConstructTower::onAttached(GameContext* gameContext)
  {
  transferEnergyTimer.setTimeOut(TOGameContext::cast(gameContext)->timeBetweenEnergyTransfers());
  transferEnergyTimer.reset();
  createRenderables(gameContext);
  setConstructionProgress(-1);
  }

void UnderConstructTower::onUpdate(GameContext* gameContext)
  {
  TOGameContext* toGameContext = TOGameContext::cast(gameContext);
  Tower::onUpdate(gameContext);

  if (transferEnergyTimer.incrementTimer(gameContext->getDeltaTime()))
    {
    TowerPtr targetTower = toGameContext->findClosestConnectedPowerSrc(getID(), true);
    if (targetTower)
      {
      uint energyAmount = (uint)((energyTransferRate / 1000.0f) * toGameContext->timeBetweenEnergyTransfers());
      toGameContext->transferEnergy(targetTower.get(), this, energyAmount);
      setConstructionProgress((float)getStoredEnergy() / (float)getMaxEnergy());
      }
    transferEnergyTimer.setTimeOut(toGameContext->timeBetweenEnergyTransfers());
    transferEnergyTimer.reset();

    if (getStoredEnergy() >= getMaxEnergy())
      {
      toGameContext->removeTower(getID());
      TowerPtr constructedTower = toGameContext->createTower(towerType, getPosition(), false);
      }
    }
  }

void UnderConstructTower::onDetached(GameContext* gameContext)
  {
  Tower::onDetached(gameContext);

  if (towerBaseWF)
    {
    gameContext->getRenderContext()->getRenderableSet()->removeRenderable(towerBaseWF->getID());
    towerBaseWF->cleanUp(gameContext->getRenderContext());
    towerBaseWF.reset();
    }
  if (towerTurretWF)
    {
    gameContext->getRenderContext()->getRenderableSet()->removeRenderable(towerTurretWF->getID());
    towerTurretWF->cleanUp(gameContext->getRenderContext());
    towerTurretWF.reset();
    }
  }

void UnderConstructTower::setConstructionProgress(float progressFactor)
  {
  if (towerHeight > 0)
    {
    Vector4D plane = Vector4D(0, -1, 0, progressFactor * towerHeight);
    if (towerBase)
      towerBase->setClippingPlane(plane);
    if (towerTurret)
      towerTurret->setClippingPlane(plane);

    Vector4D inversePlane = Vector4D(0, -1 * plane.y, 0, -1 * plane.w);
    if (towerBaseWF)
      towerBaseWF->setClippingPlane(inversePlane);
    if (towerTurretWF)
      towerTurretWF->setClippingPlane(inversePlane);
    }
  }

Tower::TowerFunction UnderConstructTower::getFunction() const
  {
  return TowerFactory::getTowerFunction(towerType);
  }

RenderablePtr UnderConstructTower::createTowerMesh(MeshStoragePtr& meshStorage, RenderContext* context)
  {
  PhasingMesh* towerMesh = new PhasingMesh(context->getNextRenderableID(), false);
  towerMesh->setMeshStorage(meshStorage);
  towerMesh->getTransform()->translate(position);
  towerMesh->setPhaseSpeed(-0.007f);
  towerMesh->setDrawStyleSingleColour(phasingColour);
  towerMesh->initialise(context);
  RenderablePtr meshPtr(towerMesh);
  context->getRenderableSet()->addRenderable(meshPtr);
  return meshPtr;
  }

RenderablePtr UnderConstructTower::createTowerMeshWF(MeshStoragePtr& meshStorage, RenderContext* context)
  {
  RenderableMesh* towerMeshWF = new RenderableMesh(context->getNextRenderableID());
  towerMeshWF->setMeshStorage(meshStorage);
  towerMeshWF->initialise(context);
  towerMeshWF->getTransform()->translate(position);
  towerMeshWF->setWireframeMode(true);
  towerMeshWF->setDrawStyleSingleColour(wfColour);
  towerMeshWF->setLightShaded(false);
  towerMeshWF->setTransparency(0.3);
  RenderablePtr meshPtr(towerMeshWF);
  context->getRenderableSet()->addRenderable(meshPtr);
  return meshPtr;
  }

void UnderConstructTower::createRenderables(GameContext* gameContext)
  {
  RenderContext* context = gameContext->getRenderContext();
  towerHeight = -1;

  //  tower base renderable
  if (!baseModelFile.empty())
    {
    MeshStoragePtr meshStorage = context->getSharedMeshStorage(baseModelFile);
    if (meshStorage)
      {
      towerBase = createTowerMesh(meshStorage, context);
      towerBaseWF = createTowerMeshWF(meshStorage, context);
      towerHeight = (float)meshStorage->getMax().y;
      }
    }

  //  tower turret renderable
  if (!turretModelFile.empty())
    {
    MeshStoragePtr meshStorage = context->getSharedMeshStorage(turretModelFile);
    if (meshStorage)
      {
      towerTurret = createTowerMesh(meshStorage, context);
      towerTurretWF = createTowerMeshWF(meshStorage, context);
      towerHeight = std::max(towerHeight, (float)meshStorage->getMax().y);
      }
    }
  }


