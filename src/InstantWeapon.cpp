//
// Created by matt on 10/12/17.
//

#include <RenderSystem/RenderableMesh.h>
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

    if (TowerPtr targetPtr = getTarget())
      {
      sourceTower->takeEnergy(energyPerShot, true);
      targetPtr->inflictDamage(damagePerShot);
      createBeamShot(context, shootPos, targetPtr->getTargetPosition());
      }
    }
  return true;
  }

void InstantWeapon::updateIdle(GameContext* context, Tower* sourceTower)
  {
  cooldownTimer.incrementTimer(context->getDeltaTime());
  }

void InstantWeapon::endShooting(GameContext* context, Tower* sourceTower, const Vector3D& shootPos)
  {

  }

void InstantWeapon::createBeamShot(GameContext* context, const Vector3D& shootPos, const Vector3D& targetPos)
  {
  RenderContext* renderContext = context->getRenderContext();
  RenderableMesh* mesh = new RenderableMesh(renderContext->getNextRenderableID());
  mesh->setMeshStorage(renderContext->createMeshStorage(MESH_BEAM));
  mesh->initialise(renderContext);

  Vector3D shootToTarget = targetPos - shootPos;
  shootToTarget.y = 0;
  Vector3D shootDir = shootToTarget;
  shootDir.makeUniform();
  mesh->getTransform()->scale(beamRadius, beamRadius, shootToTarget.magnitude());
  float angle = mathernogl::ccwAngleBetween(Vector2D(0, -1), Vector2D(shootToTarget.x, shootToTarget.z));
  mesh->getTransform()->rotate(Vector3D(0, 1, 0), angle);
  mesh->getTransform()->translate(shootPos + shootToTarget * 0.5f);
  mesh->setLightShaded(false);
  mesh->setDrawStyleSingleColour(Vector3D(0.5, 0.15, 0.1));

  RenderablePtr meshPtr(mesh);
  renderContext->getRenderableSet()->addRenderable(meshPtr);
  TimeToLiveActor* actor = new TimeToLiveActor(context->getNextActorID(), 100);
  actor->addRenderable(meshPtr);
  context->addActor(GameActorPtr(actor));
  }

bool InstantWeapon::isCoolingDown()
  {
  return !cooldownTimer.timedOut();
  }
