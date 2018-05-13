//
// Created by matt on 12/05/18.
//

#include "TrackingWeapon.h"

bool TrackingWeapon::updateShooting(GameContext* context, Tower* sourceTower, const Vector3D& shootPos)
  {
  if (DiscreteWeapon::updateShooting(context, sourceTower, shootPos))
    {
    if (std::shared_ptr<TargetTracker> tracker = currentTargetTracker.lock())
      {
      if (TowerTargetPtr target = getTarget())
        tracker->updateTargetPosition(target->getTargetPosition());
      }
    return true;
    }
  return false;
  }

void TrackingWeapon::onShootTarget(GameContext* context, const Vector3D& shootPos, TowerTargetPtr target)
  {
  DiscreteWeapon::onShootTarget(context, shootPos, target);
  if (createTrackerFunction)
    currentTargetTracker = createTrackerFunction(context, shootPos, target);
  }
