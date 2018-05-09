#pragma once

#include <GameSystem/Timer.h>
#include "Tower.h"

/*
*   A tower weapon that causes "instant" damage to its target
*/

typedef std::function<void(GameContext* context, const Vector3D& shootPos, const Vector3D& targetPos)> DisplayShootEffect;

class InstantWeapon : public TowerWeapon
  {
private:
  Timer cooldownTimer;
  int damagePerShot = 10;
  uint energyPerShot = 2;
  DisplayShootEffect shootEffectFunction;

public:
  virtual void initShooting(GameContext* context, Tower* sourceTower) override;
  virtual void updateIdle(GameContext* context, Tower* sourceTower) override;
  virtual bool updateShooting(GameContext* context, Tower* sourceTower, const Vector3D& shootPos) override;
  virtual void endShooting(GameContext* context, Tower* sourceTower) override;

  void setCooldownTime(long time) { cooldownTimer.setTimeOut(time); }
  void setDamagePerShot(int damagePerShot) { this->damagePerShot = damagePerShot; }
  void setEnergyPerShot(uint energy) { energyPerShot = energy; }
  void setShootEffectFunction(DisplayShootEffect function) { this->shootEffectFunction = function; }

  virtual bool isCoolingDown() override;
  };
