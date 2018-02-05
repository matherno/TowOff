#pragma once

#include "Tower.h"

class TowerFunctionalityCombat : public TowerFunctionality
  {
private:
  bool isShooting = false;
  std::unique_ptr<TowerWeapon> weapon;
  Vector3D shootOffset;

public:
  TowerFunctionalityCombat() : TowerFunctionality(Tower::combat) {}
  void setShootOffset(const Vector3D& offset) { shootOffset = offset; }
  void setWeapon(std::unique_ptr<TowerWeapon> weapon) { this->weapon = std::move(weapon); }

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
public:
  TowerFunctionalityStorage() : TowerFunctionality(Tower::storage) {}
  };

class TowerFunctionalityMiner : public TowerFunctionality
  {
public:
  TowerFunctionalityMiner() : TowerFunctionality(Tower::miner) {}
  };