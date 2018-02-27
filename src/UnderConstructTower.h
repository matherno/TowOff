#pragma once

#include <GameSystem/GameSystem.h>
#include <GameSystem/Timer.h>
#include "Tower.h"

/*
*   
*/

class UnderConstructTower : public Tower
  {
private:
  float towerHeight = -1;
  RenderablePtr towerBaseWF;
  RenderablePtr towerTurretWF;
  Vector3D phasingColour = Vector3D(0, 0.45, 0.55);
  Vector3D wfColour = Vector3D(0, 0.35, 0.45);
  Timer transferEnergyTimer;
  float energyTransferRate = 25;   // base value of how much energy per second this takes from power sources

public:
  UnderConstructTower(uint id, uint towerType);
  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;
  virtual TowerFunction getFunction() const override;
  void setEnergyTransferRate(float rate) { energyTransferRate = std::max(rate, 0.0f); }
  virtual bool isUnderConstruction() const override { return true; }

protected:
  void setConstructionProgress(float progressFactor);
  void createRenderables(GameContext* gameContext);
  RenderablePtr createTowerMesh(MeshStoragePtr& meshStorage, RenderContext* context);
  RenderablePtr createTowerMeshWF(MeshStoragePtr& meshStorage, RenderContext* context);
  };
