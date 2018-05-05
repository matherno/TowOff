//
// Created by matt on 10/12/17.
//

#include <RenderSystem/RenderableMesh.h>
#include <RenderSystem/RenderableLines.h>
#include "InstantWeapon.h"
#include "TimeToLiveActor.h"
#include "Resources.h"

void InstantWeapon::initShooting(GameContext* context, Tower* sourceTower)
  {
  }

bool InstantWeapon::updateShooting(GameContext* context, Tower* sourceTower, const Vector3D& shootPos)
  {
  if (cooldownTimer.incrementTimer(context->getDeltaTime()))
    {
    cooldownTimer.reset();
    if (sourceTower->getStoredEnergy() < energyPerShot)
      return false;

    if (TowerTargetPtr targetPtr = getTarget())
      {
      sourceTower->takeEnergy(energyPerShot, true);
      targetPtr->doDamage(damagePerShot);
      createBeamShot(context, shootPos, targetPtr->getTargetPosition());
      }
    }
  return true;
  }

void InstantWeapon::updateIdle(GameContext* context, Tower* sourceTower)
  {
  cooldownTimer.incrementTimer(context->getDeltaTime());
  }

void InstantWeapon::endShooting(GameContext* context, Tower* sourceTower)
  {

  }

void InstantWeapon::createBeamShot(GameContext* context, const Vector3D& shootPos, const Vector3D& targetPos)
  {
  RenderContext* renderContext = context->getRenderContext();
  RenderableLines* line = new RenderableLines(renderContext->getNextRenderableID());
  line->initialise(renderContext);
  line->addLine(shootPos, targetPos, Vector3D(0.5, 0.1, 0.1));

  RenderablePtr linePtr(line);
  renderContext->getRenderableSet()->addRenderable(linePtr);
  TimeToLiveActor* actor = new TimeToLiveActor(context->getNextActorID(), 60);
  actor->addRenderable(linePtr);
  context->addActor(GameActorPtr(actor));
  }

bool InstantWeapon::isCoolingDown()
  {
  return !cooldownTimer.timedOut();
  }
