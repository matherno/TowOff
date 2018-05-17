#pragma once

#include <GameSystem/Timer.h>
#include "Tower.h"

/*
 *  The type of weapon that fires in a discrete manor, rather than a continuous shot
 *    e.g. A projectile weapon
 */

typedef std::function<void(GameContext* context, const Vector3D& shootPos, const Vector3D& targetPos)> OnShootFunction;

class DiscreteWeapon : public TowerWeapon
  {
private:
  Timer cooldownTimer;
  long cooldownTime = 1000;
  int damagePerShot = 10;
  uint energyPerShot = 2;
  OnShootFunction onShootFunction;
  uint burstNumShots = 1;
  long burstTimeBetweenShots = 0;
  uint currentBurstShot = 0;

public:
  virtual void initShooting(GameContext* context, Tower* sourceTower) override;
  virtual void updateIdle(GameContext* context, Tower* sourceTower) override;
  virtual bool updateShooting(GameContext* context, Tower* sourceTower, const Vector3D& shootPos) override;
  virtual void endShooting(GameContext* context, Tower* sourceTower) override;

  void setCooldownTime(long time) { cooldownTime = time; cooldownTimer.setTimeOut(time); cooldownTimer.reset(); }
  void setBurstParams(uint numShots, long timeBetweenShots);
  void setDamagePerShot(int damagePerShot) { this->damagePerShot = damagePerShot; }
  void setEnergyPerShot(uint energy) { energyPerShot = energy; }
  void setOnShootFunction(OnShootFunction function) { this->onShootFunction = function; }

  virtual bool isCoolingDown() override;

protected:
  virtual void onShootTarget(GameContext* context, const Vector3D& shootPos, TowerTargetPtr target);
  };
