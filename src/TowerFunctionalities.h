#pragma once

#include <src/GameSystem/Timer.h>
#include "Tower.h"

class TowerFunctionalityCombat : public TowerFunctionality
  {
private:
  bool isShooting = false;
  std::unique_ptr<TowerWeapon> weapon;
  Vector3D shootOffset;
  Timer transferEnergyTimer;
  float energyTransferRate = 2;   // base value of how much energy per second this takes from power sources

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
  float energyTransferRate = 10;   // base value of how much energy per second this takes from miners

public:
  TowerFunctionalityStorage() : TowerFunctionality(Tower::storage) {}
  void setEnergyTransferRate(float rate);

  virtual void onAttached(Tower* tower, GameContext* gameContext) override;
  virtual void onUpdate(Tower* tower, GameContext* gameContext) override;
  };

class TowerFunctionalityMiner : public TowerFunctionality
  {
private:
  Timer transferEnergyTimer;
  float energyTransferRate = 20;   // base value of how much energy per second this takes from resources

public:
  TowerFunctionalityMiner() : TowerFunctionality(Tower::miner) {}
  void setEnergyTransferRate(float rate);

  virtual void onAttached(Tower* tower, GameContext* gameContext) override;
  virtual void onUpdate(Tower* tower, GameContext* gameContext) override;
  };