#pragma once

#include <GameSystem/Timer.h>
#include "Tower.h"

/*
*   A tower weapon that causes "instant" damage to its target
*/

class InstantWeapon : public TowerWeapon
  {
private:
  Timer cooldownTimer;
  int damagePerShot = 10;
  float beamRadius = 0.3;
  uint energyPerShot = 2;

public:
  virtual void initShooting(GameContext* context, Tower* sourceTower) override;
  virtual void updateIdle(GameContext* context, Tower* sourceTower) override;
  virtual bool updateShooting(GameContext* context, Tower* sourceTower, const Vector3D& shootPos) override;
  virtual void endShooting(GameContext* context, Tower* sourceTower, const Vector3D& shootPos) override;

  void setCooldownTime(long time) { cooldownTimer.setTimeOut(time); }
  void setDamagePerShot(int damagePerShot) { this->damagePerShot = damagePerShot; }
  void setBeamRadius(float beamRadius) { this->beamRadius = beamRadius; }
  void setEnergyPerShot(uint energy) { energyPerShot = energy; }

  virtual bool isCoolingDown() override;

protected:
  void createBeamShot(GameContext* context, const Vector3D& shootPos, const Vector3D& targetPos);
  };
