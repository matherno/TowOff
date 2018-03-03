#pragma once

#include <GameSystem/GameSystem.h>
#include <ParticleSystem/ParticleSystem.h>
#include <set>
#include "Tower.h"

/*
*   
*/

class SpecialEffectsHandler
  {
private:
  ParticleSystemPtr towerDamageParticles;
  ParticleSystemPtr towerSmokeParticles;
  std::set<uint> towersEmittingSmoke;

public:
  SpecialEffectsHandler();
  void initialise(GameContext* gameContext);
  void update(GameContext* gameContext);
  void cleanUp(GameContext* gameContext);

  void towerDamageEffect(GameContext* gameContext, const Tower* tower, const Vector3D& effectColour);

protected:
  void startSmoke(TowerPtr tower);
  void endSmoke(TowerPtr tower);
  bool isSmoking(TowerPtr tower) const;
  };
