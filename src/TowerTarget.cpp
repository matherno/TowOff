//
// Created by matt on 29/04/18.
//

#include "TowerTarget.h"
#include "TOGameContext.h"

TowerTarget::TowerTarget(uint id) : GameActor(id)
  {}

void TowerTarget::onAttached(GameContext* gameContext)
  {

  }

void TowerTarget::onUpdate(GameContext* gameContext)
  {

  }

void TowerTarget::onDetached(GameContext* gameContext)
  {

  }

void TowerTarget::setMaxHealthPoints(uint max)
  {
  maxHealthPoints = max;
  if (healthPoints > max)
    healthPoints = max;
  }

bool TowerTarget::doDamage(uint damagePoints)
  {
  if (healthPoints <= damagePoints)
    {
    healthPoints = 0;
    return true;
    }
  healthPoints -= damagePoints;
  return false;
  }
