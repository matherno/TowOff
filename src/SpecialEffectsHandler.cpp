//
// Created by matt on 3/03/18.
//

#include "SpecialEffectsHandler.h"
#include "TOGameContext.h"

#define HEALTH_TO_EMIT_SMOKE   0.6f     //  when a tower is at or under this health factor, it'll emit smoke

SpecialEffectsHandler::SpecialEffectsHandler()
  {}

void SpecialEffectsHandler::initialise(GameContext* gameContext)
  {
  {
    ParticleSystem* system = new ParticleSystem(gameContext->getNextActorID(), true);
    system->setGravityAccel(0.00001);
    system->setTimeBetweenSpawns(5);
    system->setInitVelocity(0.008);
    system->setTimeAlive(300);
    system->setParticleSize(4);
    towerDamageParticles.reset(system);
    gameContext->addActor(towerDamageParticles);
  }

  {
    ParticleSystem* system = new ParticleSystem(gameContext->getNextActorID(), true);
    system->setGravityAccel(0.000002);
    system->setTimeBetweenSpawns(150);
    system->setInitVelocity(0.004);
    system->setTimeAlive(1800);
    system->setParticleSize(4);
    system->setParticleDirectionHemisphere(Vector3D(0, 1, 0), 200);
    system->setParticleSpawnSphere(0.5);
    towerSmokeParticles.reset(system);
    gameContext->addActor(towerSmokeParticles);
  }

  }

void SpecialEffectsHandler::update(GameContext* gameContext)
  {
  TOGameContext* toGameContext = TOGameContext::cast(gameContext);
  for (TowerPtr tower : *toGameContext->getTowers()->getList())
    {
    bool emittingSmoke = (HEALTH_TO_EMIT_SMOKE - tower->getHealthFactor()) / HEALTH_TO_EMIT_SMOKE > 0;
    if (emittingSmoke)
      startSmoke(tower);
    else
      endSmoke(tower);
    }
  }

void SpecialEffectsHandler::cleanUp(GameContext* gameContext)
  {
  gameContext->removeActor(towerDamageParticles->getID());
  gameContext->removeActor(towerSmokeParticles->getID());
  }

void SpecialEffectsHandler::towerDamageEffect(GameContext* gameContext, const Tower* tower, const Vector3D& effectColour)
  {
  towerDamageParticles->addEmitter(tower->getTargetPosition(), 50, effectColour);
  }

void SpecialEffectsHandler::startSmoke(TowerPtr tower)
  {
  if (!isSmoking(tower))
    {
    towersEmittingSmoke.insert(tower->getID());
    towerSmokeParticles->addEmitter(tower->getPosition() + Vector3D(0, 0, 0), [this, tower]()
      {
      if (!isSmoking(tower))
        return true;
      if (!tower->isAlive())
        {
        endSmoke(tower);
        return true;
        }
      return false;
      }, Vector3D(0.1, 0.1, 0.1));
    }
  }

void SpecialEffectsHandler::endSmoke(TowerPtr tower)
  {
  if (isSmoking(tower))
    towersEmittingSmoke.erase(tower->getID());
  }

bool SpecialEffectsHandler::isSmoking(TowerPtr tower) const
  {
  return towersEmittingSmoke.count(tower->getID()) > 0;
  }
