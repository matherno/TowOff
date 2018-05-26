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

  const float towerRange = TowerFactory::getCombatRange(tower->getTowerType());
  const float towerMinRange = TowerFactory::getCombatMinRange(tower->getTowerType());

  if (!isShooting)
    {
    if (!weapon)
      return;

    weapon->updateIdle(toGameContext, tower);
    if (weapon->isCoolingDown())
      return;

    TowerTargetPtr target = toGameContext->findClosestTowerTarget(tower->getPosition(), towerRange, towerMinRange);
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
      if (target->isInRange(tower->getPosition(), towerRange, towerMinRange))
        {
        tower->setTurretRotation(target->getPosition());
        Vector3D shootPos = tower->getPosition() + shootOffset;
        tower->getTurretTransform()->transform(shootOffset, &shootPos);
        stopShooting = !weapon->updateShooting(toGameContext, tower, shootPos);
        }
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

  RenderContext* renderContext = gameContext->getRenderContext();
  miningBeams.reset(new RenderableLines(renderContext->getNextRenderableID()));
  miningBeams->setLineWidth(1.5);
  miningBeams->initialise(renderContext);
  renderContext->getRenderableSet()->addRenderable(miningBeams);
  }

void TowerFunctionalityMiner::onDetached(Tower* tower, GameContext* gameContext)
  {
  miningBeams->cleanUp(gameContext->getRenderContext());
  gameContext->getRenderContext()->getRenderableSet()->removeRenderable(miningBeams->getID());
  miningBeams.reset();
  }

void TowerFunctionalityMiner::onUpdate(Tower* tower, GameContext* gameContext)
  {
  if (transferEnergyTimer.incrementTimer(gameContext->getDeltaTime()))
    {
    TOGameContext* toGameContext = TOGameContext::cast(gameContext);
    uint energyToMine = (uint) ((energyTransferRate / 1000.0f) * toGameContext->timeBetweenEnergyTransfers());

    DepositPtr deposit = targetDeposit.lock();
    if (!deposit)
      {
      //  find new deposit to mine
      const float range = TowerFactory::getTowerRange(tower->getTowerType());
      deposit = toGameContext->findClosestDeposit(tower->getPosition(), energyToMine, range);
      targetDeposit = deposit;
      }

    if (deposit)
      {
      //  stop mining if deposit is depleted
      if (deposit->getStoredEnergyAmount() < energyToMine)
        {
        targetDeposit.reset();
        miningBeams->clearLines();
        stopMiningBeams();
        return;
        }

      //  mine energy
      energyToMine = std::min(tower->getAvailableEnergyStorage(), energyToMine);
      if (energyToMine > 0)
        {
        energyToMine = deposit->takeEnergy(energyToMine, false);
        tower->storeEnergy(energyToMine);
        transferEnergyTimer.setTimeOut(toGameContext->timeBetweenEnergyTransfers());
        transferEnergyTimer.reset();
        tower->setTurretRotation(deposit->getPosition());
        startMiningBeams(tower, deposit.get());
        }
      else
        {
        stopMiningBeams();
        }
      }
    }
  }

void TowerFunctionalityMiner::setEnergyTransferRate(float rate)
  {
  energyTransferRate = std::max(rate, 0.0f);
  }

void TowerFunctionalityMiner::startMiningBeams(Tower* tower, Deposit* deposit)
  {
  if (beamsActive)
    return;

  miningBeams->disableRebuild();
  miningBeams->clearLines();
  const Transform* transform = tower->getTurretTransform();
  for (const Vector3D& beamStart : miningBeamOffsets)
    {
    const float offset = 0.1f;
    for (const Vector2D& beamOffset : {Vector2D(0, offset), Vector2D(0, -offset), Vector2D(offset, 0), Vector2D(-offset, 0)})
      {
      Vector3D transformedStart = beamStart;
      if (transform)
        transformedStart = transform->transform(beamStart + beamOffset);
      else
        transformedStart += tower->getPosition() + beamOffset;
      miningBeams->addLine(transformedStart, deposit->getPosition(), beamColour);
      }
    }
  miningBeams->enableRebuild();
  miningBeams->rebuildBuffer();
  beamsActive = true;
  }

void TowerFunctionalityMiner::stopMiningBeams()
  {
  if (beamsActive)
    miningBeams->clearLines();
  beamsActive = false;
  }