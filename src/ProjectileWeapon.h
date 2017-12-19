#pragma once

#include "Tower.h"
#include "Projectile.h"

/*
*   A tower weapon that launches a projectile at its target
*/

typedef std::function<ProjectilePtr(uint id)> CreateProjectileFunc;

class ProjectileWeapon : public TowerWeapon
  {
private:
  long lastShootTime = 0;

  CreateProjectileFunc createProjectileFunc;
  long cooldownTime = 1000;
  float shootForce = 20;
  float gravityForce = 2;

public:
  virtual void initShooting(GameContext* context) override;
  virtual void updateShooting(GameContext* context, const Vector3D& shootPos) override;
  virtual void endShooting(GameContext* context, const Vector3D& shootPos) override;

  void setCreateProjectileFunc(CreateProjectileFunc func){ createProjectileFunc = func; }
  void setCooldownTime(long cooldownTime) { this->cooldownTime = cooldownTime; }
  void setShootForce(double shootForce) { this->shootForce = shootForce; }
  void setGravityForce(double gravityForce) { this->gravityForce = gravityForce; }
  };
