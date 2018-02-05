//
// Created by matt on 5/02/18.
//

#include "TowerFunctionalities.h"
#include "TOGameContext.h"

void TowerFunctionalityCombat::onUpdate(Tower* tower, GameContext* gameContext)
  {
  TOGameContext* toGameContext = TOGameContext::cast(gameContext);

  if (!isShooting)
    {
    if (!weapon || weapon->isCoolingDown(gameContext->getGameTime()))
      return;

    if (!toGameContext->isConnectedToPowerSrc(tower->getID()))
      return;

    TowerPtr target = toGameContext->getClosestTowerTo(tower, true);
    if (target && weapon)
      {
      weapon->setTarget(target);
      weapon->initShooting(toGameContext);
      tower->setTurretRotation(target->getTargetPosition());
      isShooting = true;
      }
    }
  else
    {
    Vector3D shootPos = tower->getPosition() + shootOffset;
    if (tower->getTurretRotation())
      tower->getTurretRotation()->transform(shootOffset, &shootPos);
    bool connectedToSrc = toGameContext->isConnectedToPowerSrc(tower->getID());
    if (weapon->getTarget() && weapon->getTarget()->isAlive() && connectedToSrc)
      {
      weapon->updateShooting(toGameContext, shootPos);
      }
    else
      {
      weapon->endShooting(toGameContext, shootPos);
      isShooting = false;
      }
    }
  }

void TowerFunctionalityCombat::onDetached(Tower* tower, GameContext* gameContext)
  {
  if (isShooting && weapon)
    weapon->endShooting(TOGameContext::cast(gameContext), tower->getPosition() + shootOffset);
  }
