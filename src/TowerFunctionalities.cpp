//
// Created by matt on 5/02/18.
//

#include "TowerFunctionalities.h"
#include "TOGameContext.h"
#include "TowerFactory.h"

void TowerFunctionalityCombat::onAttached(Tower* tower, GameContext* gameContext)
  {
  transferEnergyTimer.setTimeOut(TOGameContext::cast(gameContext)->timeBetweenEnergyTransfers());
  }

void TowerFunctionalityCombat::onUpdate(Tower* tower, GameContext* gameContext)
  {
  TOGameContext* toGameContext = TOGameContext::cast(gameContext);

  if (transferEnergyTimer.incrementTimer(gameContext->getDeltaTime()))
    {
    TowerPtr powerSrcTower = toGameContext->findClosestConnectedPowerSrc(tower->getID(), true);
    if (powerSrcTower)
      {
      uint energyAmount = (uint)((energyTransferRate / 1000.0f) * toGameContext->timeBetweenEnergyTransfers());
      toGameContext->transferEnergy(powerSrcTower.get(), tower, energyAmount);
      }
    transferEnergyTimer.setTimeOut(toGameContext->timeBetweenEnergyTransfers());
    transferEnergyTimer.reset();
    }

  if (!isShooting)
    {
    if (!weapon)
      return;

    weapon->updateIdle(toGameContext, tower);
    if (weapon->isCoolingDown())
      return;

    const float towerRange = TowerFactory::getCombatRange(tower->getTowerType());
    TowerTargetPtr target = toGameContext->findClosestTowerTarget(tower->getPosition(), towerRange);
    if (target && weapon)
      {
      weapon->setTarget(target);
      weapon->initShooting(toGameContext, tower);
      tower->setTurretRotation(target->getPosition());
      isShooting = true;
      }
    }
  else
    {
    bool stopShooting = true;
    TowerTargetPtr target = weapon->getTarget();
    if (target && target->isAlive())
      {
      tower->setTurretRotation(target->getPosition());
      Vector3D shootPos = tower->getPosition() + shootOffset;
      tower->getTurretRotation()->transform(shootOffset, &shootPos);
      stopShooting = !weapon->updateShooting(toGameContext, tower, shootPos);
      }

    if (stopShooting)
      {
      weapon->endShooting(toGameContext, tower);
      isShooting = false;
      }
    }
  }

void TowerFunctionalityCombat::onDetached(Tower* tower, GameContext* gameContext)
  {
  if (isShooting && weapon)
    weapon->endShooting(TOGameContext::cast(gameContext), tower);
  }

void TowerFunctionalityCombat::setEnergyTransferRate(float rate)
  {
  energyTransferRate = std::max(rate, 0.0f);
  }

void TowerFunctionalityStorage::onAttached(Tower* tower, GameContext* gameContext)
  {
  transferEnergyTimer.setTimeOut(TOGameContext::cast(gameContext)->timeBetweenEnergyTransfers());
  }

void TowerFunctionalityStorage::onUpdate(Tower* tower, GameContext* gameContext)
  {
  TOGameContext* toGameContext = TOGameContext::cast(gameContext);
  if (transferEnergyTimer.incrementTimer(gameContext->getDeltaTime()))
    {
    TowerPtr targetTower = toGameContext->findClosestConnectedMiner(tower->getID(), true);
    if (targetTower)
      {
      uint energyAmount = (uint)((energyTransferRate / 1000.0f) * toGameContext->timeBetweenEnergyTransfers());
      toGameContext->transferEnergy(targetTower.get(), tower, energyAmount);
      }
    transferEnergyTimer.setTimeOut(toGameContext->timeBetweenEnergyTransfers());
    transferEnergyTimer.reset();
    }
  }

void TowerFunctionalityStorage::setEnergyTransferRate(float rate)
  {
  energyTransferRate = std::max(rate, 0.0f);
  }

void TowerFunctionalityMiner::onAttached(Tower* tower, GameContext* gameContext)
  {
  transferEnergyTimer.setTimeOut(TOGameContext::cast(gameContext)->timeBetweenEnergyTransfers());
  }

void TowerFunctionalityMiner::onUpdate(Tower* tower, GameContext* gameContext)
  {
  if (transferEnergyTimer.incrementTimer(gameContext->getDeltaTime()))
    {
    TOGameContext* toGameContext = TOGameContext::cast(gameContext);
    uint energyAmount = (uint)((energyTransferRate / 1000.0f) * toGameContext->timeBetweenEnergyTransfers());
    tower->storeEnergy(energyAmount);
    transferEnergyTimer.setTimeOut(toGameContext->timeBetweenEnergyTransfers());
    transferEnergyTimer.reset();
    }
  }

void TowerFunctionalityMiner::setEnergyTransferRate(float rate)
  {
  energyTransferRate = std::max(rate, 0.0f);
  }
