#pragma once

#include <GameSystem/Timer.h>
#include <RenderSystem/RenderableLines.h>
#include "Tower.h"
#include "Deposit.h"

class TowerFunctionalityCombat : public TowerFunctionality
  {
private:
  bool isShooting = false;
  std::unique_ptr<TowerWeapon> weapon;
  Vector3D shootOffset;
  Timer transferEnergyTimer;
  float energyTransferRate = 15;   // base value of how much energy per second this takes from power sources

public:
  TowerFunctionalityCombat() : TowerFunctionality(Tower::combat) {}
  void setShootOffset(const Vector3D& offset) { shootOffset = offset; }
  void setWeapon(std::unique_ptr<TowerWeapon> weapon) { this->weapon = std::move(weapon); }
  void setEnergyTransferRate(float rate);

  virtual void onAttached(Tower* tower, GameContext* gameContext) override;
  virtual void onUpdate(Tower* tower, GameContext* gameContext) override;
  virtual void onDetached(Tower* tower, GameContext* gameContext) override;
  };

class TowerFunctionalityRelay : public TowerFunctionality
  {
public:
  TowerFunctionalityRelay() : TowerFunctionality(Tower::relay) {}
  };

class TowerFunctionalityStorage : public TowerFunctionality
  {
private:
  Timer transferEnergyTimer;
  float energyTransferRate = 25;   // base value of how much energy per second this takes from miners

public:
  TowerFunctionalityStorage(Tower::TowerFunction function = Tower::storage) : TowerFunctionality(function) {}
  void setEnergyTransferRate(float rate);

  virtual void onAttached(Tower* tower, GameContext* gameContext) override;
  virtual void onUpdate(Tower* tower, GameContext* gameContext) override;
  };

class TowerFunctionalityHomeBase : public TowerFunctionalityStorage
  {
public:
  TowerFunctionalityHomeBase() : TowerFunctionalityStorage(Tower::homebase) {}
  };

class TowerFunctionalityMiner : public TowerFunctionality
  {
private:
  Timer transferEnergyTimer;
  float energyTransferRate = 15;   // base value of how much energy per second this takes from resources
  std::weak_ptr<Deposit> targetDeposit;
  std::shared_ptr<RenderableLines> miningBeams;
  std::vector<Vector3D> miningBeamOffsets;    // can have any number of mining beams
  Vector3D beamColour = Vector3D(0.5, 0.3, 0.1);
  bool beamsActive = false;

public:
  TowerFunctionalityMiner() : TowerFunctionality(Tower::miner) {}
  void setEnergyTransferRate(float rate);
  void addMiningBeamOffset(const Vector3D& offset){ miningBeamOffsets.push_back(offset); }

  virtual void onAttached(Tower* tower, GameContext* gameContext) override;
  virtual void onUpdate(Tower* tower, GameContext* gameContext) override;
  virtual void onDetached(Tower* tower, GameContext* gameContext) override;

protected:
  void startMiningBeams(Tower* tower, Deposit* deposit);
  void stopMiningBeams();
  };