#pragma once

#include "Tower.h"

/*
*   A tower weapon that causes "instant" damage to its target
*/

class InstantWeapon : public TowerWeapon
  {
private:
  long cooldownTime = 1000;
  long lastShootTime = -9999;
  int damagePerShot = 10;
  float beamRadius = 0.3;

public:
  virtual void initShooting(GameContext* context) override;
  virtual void updateShooting(GameContext* context, const Vector3D& shootPos) override;
  virtual void endShooting(GameContext* context, const Vector3D& shootPos) override;

  void setCooldownTime(long time) { cooldownTime = time; }
  void setDamagePerShot(int damagePerShot) { this->damagePerShot = damagePerShot; }
  void setBeamRadius(float beamRadius) { this->beamRadius = beamRadius; }

  virtual bool isCoolingDown(long currentTime) override;

protected:
  void createBeamShot(GameContext* context, const Vector3D& shootPos, const Vector3D& targetPos);
  };
