//
// Created by matt on 10/12/17.
//

#include "InstantWeapon.h"

void InstantWeapon::initShooting(GameContext* context, Tower* sourceTower)
  {
  }

bool InstantWeapon::updateShooting(GameContext* context, Tower* sourceTower, const Vector3D& shootPos)
  {
  if (cooldownTimer.incrementTimer(context->getDeltaTime()))
    {
    cooldownTimer.reset();
    if (sourceTower->getStoredEnergy() < energyPerShot)
      return false;

    if (TowerTargetPtr targetPtr = getTarget())
      {
      sourceTower->takeEnergy(energyPerShot, true);
      targetPtr->doDamage(damagePerShot);
      if (shootEffectFunction)
        shootEffectFunction(context, shootPos, targetPtr->getTargetPosition());
      }
    }
  return true;
  }

void InstantWeapon::updateIdle(GameContext* context, Tower* sourceTower)
  {
  cooldownTimer.incrementTimer(context->getDeltaTime());
  }

void InstantWeapon::endShooting(GameContext* context, Tower* sourceTower)
  {

  }

bool InstantWeapon::isCoolingDown()
  {
  return !cooldownTimer.timedOut();
  }
