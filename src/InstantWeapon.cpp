//
// Created by matt on 10/12/17.
//

#include <src/RenderSystem/RenderableMesh.h>
#include "InstantWeapon.h"
#include "TimeToLiveActor.h"
#include "Resources.h"

void InstantWeapon::initShooting(GameContext* context)
  {
  }

void InstantWeapon::updateShooting(GameContext* context, const Vector3D& shootPos)
  {
  if (context->getGameTime() - lastShootTime > cooldownTime)
    {
    if (TowerPtr targetPtr = getTarget())
      {
      targetPtr->inflictDamage(damagePerShot);
      createBeamShot(context, shootPos, targetPtr->getTargetPosition());
      lastShootTime = context->getGameTime();
      }
    }
  }

void InstantWeapon::endShooting(GameContext* context, const Vector3D& shootPos)
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
  mesh->getTransform()->scale(0.3, 0.3, shootToTarget.magnitude());
  float angle = mathernogl::ccwAngleBetween(Vector2D(0, -1), Vector2D(shootToTarget.x, shootToTarget.z));
  mesh->getTransform()->rotate(Vector3D(0, 1, 0), angle);
  mesh->getTransform()->translate(shootPos + shootToTarget * 0.5f);
  mesh->setLightShaded(false);
  mesh->setColour(Vector3D(0.5, 0.15, 0.1));

  RenderablePtr meshPtr(mesh);
  renderContext->getRenderableSet()->addRenderable(meshPtr);
  TimeToLiveActor* actor = new TimeToLiveActor(context->getNextActorID(), 100);
  actor->addRenderable(meshPtr);
  context->addActor(GameActorPtr(actor));
  }

bool InstantWeapon::isCoolingDown(long currentTime)
  {
  return currentTime - lastShootTime <= cooldownTime;
  }
