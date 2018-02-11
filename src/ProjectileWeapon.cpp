//
// Created by matt on 13/12/17.
//

#include "ProjectileWeapon.h"

void ProjectileWeapon::initShooting(GameContext* context, Tower* sourceTower)
  {

  }

bool ProjectileWeapon::updateShooting(GameContext* context, Tower* sourceTower, const Vector3D& shootPos)
  {
  if (createProjectileFunc && context->getGameTime() - lastShootTime > cooldownTime)
    {
    if (TowerPtr targetPtr = getTarget())
      {
      targetPtr->inflictDamage(mathernogl::RandomGenerator::randomInt(9, 11));

      Vector3D shootDirection = targetPtr->getTargetPosition() - shootPos;
      shootDirection.makeUniform();

      std::shared_ptr<Projectile> projectile = createProjectileFunc(context->getNextActorID());
      projectile->setPosition(shootPos);
      projectile->setVelocity(shootDirection * shootForce);
      projectile->setAcceleration(Vector3D(0, -gravityForce, 0));
      context->addActor(projectile);

      lastShootTime = context->getGameTime();
      }
    }
  return true;
  }

void ProjectileWeapon::endShooting(GameContext* context, Tower* sourceTower, const Vector3D& shootPos)
  {

  }

bool ProjectileWeapon::isCoolingDown()
  {
  return 4 - lastShootTime <= cooldownTime;
  }
