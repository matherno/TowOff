//
// Created by matt on 10/12/17.
//

#include "DiscreteWeapon.h"

void DiscreteWeapon::initShooting(GameContext* context, Tower* sourceTower)
  {
  }

bool DiscreteWeapon::updateShooting(GameContext* context, Tower* sourceTower, const Vector3D& shootPos)
  {
  if (cooldownTimer.incrementTimer(context->getDeltaTime()))
    {
    if (sourceTower->getStoredEnergy() < energyPerShot)
      return false;

    if (TowerTargetPtr targetPtr = getTarget())
      {
      sourceTower->takeEnergy(energyPerShot, true);
      if (damagePerShot > 0)
        targetPtr->doDamage(damagePerShot);
      onShootTarget(context, shootPos, targetPtr);
      }

    if (currentBurstShot < burstNumShots - 1)
      {
      //  continue bursting
      cooldownTimer.setTimeOut(burstTimeBetweenShots);
      ++currentBurstShot;
      }
    else
      {
      //  end bursting
      cooldownTimer.setTimeOut(cooldownTime);
      currentBurstShot = 0;
      }
    cooldownTimer.reset();
    }
  return true;
  }

void DiscreteWeapon::updateIdle(GameContext* context, Tower* sourceTower)
  {
  cooldownTimer.incrementTimer(context->getDeltaTime());
  }

void DiscreteWeapon::endShooting(GameContext* context, Tower* sourceTower)
  {

  }

bool DiscreteWeapon::isCoolingDown()
  {
  return !cooldownTimer.timedOut();
  }

void DiscreteWeapon::onShootTarget(GameContext* context, const Vector3D& shootPos, TowerTargetPtr target)
  {
  if (onShootFunction)
    onShootFunction(context, shootPos, target->getTargetPosition());
  }

void DiscreteWeapon::setBurstParams(uint numShots, long timeBetweenShots)
  {
  burstNumShots = std::max(numShots, 1u);
  burstTimeBetweenShots = timeBetweenShots;
  currentBurstShot = 0;
  }
