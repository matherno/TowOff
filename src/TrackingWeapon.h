#pragma once

#include "Tower.h"
#include "DiscreteWeapon.h"

/*
*   Weapon that keeps its TargetTracker updated with the current position of its target, on each update
*/

class TargetTracker
  {
private:
  Vector3D targetPosition;

public:
  virtual void updateTargetPosition(const Vector3D& position) { targetPosition = position; }
  Vector3D getTargetPosition() const { return targetPosition; }
  };

typedef std::function<std::shared_ptr<TargetTracker>(GameContext* gameContext, const Vector3D& shootPos, TowerTargetPtr target)> CreateTargetTracker;

class TrackingWeapon : public DiscreteWeapon
  {
protected:
  std::weak_ptr<TargetTracker> currentTargetTracker;
  CreateTargetTracker createTrackerFunction;

public:
  void setCreateTrackerFunction(CreateTargetTracker func) { createTrackerFunction = func; }
  virtual bool updateShooting(GameContext* context, Tower* sourceTower, const Vector3D& shootPos) override;

protected:
  virtual void onShootTarget(GameContext* context, const Vector3D& shootPos, TowerTargetPtr target) override;
  };
