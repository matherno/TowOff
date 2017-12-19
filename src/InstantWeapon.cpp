//
// Created by matt on 10/12/17.
//

#include "InstantWeapon.h"

void InstantWeapon::initShooting(GameContext* context)
  {
  }

void InstantWeapon::updateShooting(GameContext* context, const Vector3D& shootPos)
  {
  if (context->getGameTime() - lastShootTime > cooldownTime)
    {
    if (TowerPtr targetPtr = getTarget())
      {
      targetPtr->inflictDamage(damagePerShot);
      lastShootTime = context->getGameTime();
      }
    }
  }

void InstantWeapon::endShooting(GameContext* context, const Vector3D& shootPos)
  {

  }
