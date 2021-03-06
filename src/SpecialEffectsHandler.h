#pragma once

#include <GameSystem/GameSystem.h>
#include <ParticleSystem/ParticleSystem.h>
#include <set>
#include "Tower.h"
#include "Bot.h"

/*
*   
*/

class SpecialEffectsHandler
  {
private:
  ParticleSystemPtr towerDamageParticles;
  ParticleSystemPtr botDamageParticles;
  ParticleSystemPtr towerSmokeParticles;
  ParticleSystemPtr explosionParticles;
  std::set<uint> towersEmittingSmoke;
  Vector3D towerDamageColour;
  Vector3D botDamageColour;

public:
  SpecialEffectsHandler();
  void initialise(GameContext* gameContext);
  void update(GameContext* gameContext);
  void cleanUp(GameContext* gameContext);

  void towerDamageEffect(GameContext* gameContext, const Tower* tower);
  void botDamageEffect(GameContext* gameContext, const Bot* bot);
  void botExplosionEffect(GameContext* gameContext, const Bot* bot);
  void weaponExplosionEffect(GameContext* gameContext, const Vector3D& position, float scale);

protected:
  void startSmoke(TowerPtr tower);
  void endSmoke(TowerPtr tower);
  bool isSmoking(TowerPtr tower) const;
  };
