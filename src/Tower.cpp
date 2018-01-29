//
// Created by matt on 3/12/17.
//

#include <RenderSystem/RenderableMesh.h>
#include "Tower.h"
#include "TOGameContext.h"
#include "InstantWeapon.h"
#include "ProjectileWeapon.h"

Tower::Tower(uint id, string baseModelFile, string turretModelFile)
  : GameActor(id), baseModelFile(baseModelFile), turretModelFile(turretModelFile)
  {
  }

void Tower::onAttached(GameContext* gameContext)
  {
  RenderContext* context = gameContext->getRenderContext();

  //  tower base renderable
  if (!baseModelFile.empty())
    {
    MeshStoragePtr meshStorage = context->createMeshStorage(baseModelFile);
    if (meshStorage)
      {
      RenderableMesh* renderable = new RenderableMesh(context->getNextRenderableID());
      renderable->setMeshStorage(MeshStoragePtr(meshStorage));
      renderable->initialise(context);
      renderable->getTransform()->translate(position);
      towerBase = RenderablePtr(renderable);
      context->getRenderableSet()->addRenderable(towerBase);
      }
    }

  //  tower turret renderable
  if (!turretModelFile.empty())
    {
    MeshStoragePtr meshStorage = context->createMeshStorage(turretModelFile);
    if (meshStorage)
      {
      RenderableMesh* renderable = new RenderableMesh(context->getNextRenderableID());
      renderable->setMeshStorage(MeshStoragePtr(meshStorage));
      renderable->initialise(context);
      renderable->getTransform()->translate(position);
      towerTurret = RenderablePtr(renderable);
      context->getRenderableSet()->addRenderable(towerTurret);
      }
    }
  }

void Tower::onUpdate(GameContext* gameContext)
  {
  TOGameContext* toGameContext = TOGameContext::cast(gameContext);
  if (showDamageEffect)
    {
    toGameContext->doTowerDamageEffect(this);
    showDamageEffect = false;
    }

  if (!isAlive())
    {
    toGameContext->removeTower(getID());
    return;
    }

  ((RenderableMesh*)towerBase.get())->setColour(toGameContext->getPlayerColour(getPlayerNum()));

  if (state == idle)
    {
    if (weapon && weapon->isCoolingDown(gameContext->getGameTime()))
      return;

    TowerPtr target = toGameContext->getClosestTowerTo(this, true);
    if (target && weapon)
      {
      weapon->setTarget(target);
      weapon->initShooting(toGameContext);
      setTurretRotation(target->getTargetPosition());
      state = shooting;
      }
    }
  else if (state == shooting)
    {
    Vector3D shootPos = position + shootOffset;
    if (towerTurret)
      towerTurret->getTransform()->transform(shootOffset, &shootPos);
    if (weapon->getTarget() && weapon->getTarget()->isAlive())
      {
      weapon->updateShooting(toGameContext, shootPos);
      }
    else
      {
      weapon->endShooting(toGameContext, shootPos);
      state = idle;
      }
    }
  }

void Tower::onDetached(GameContext* gameContext)
  {
  if (state == shooting && weapon)
    weapon->endShooting(TOGameContext::cast(gameContext), position + shootOffset);
  if (towerBase)
    {
    gameContext->getRenderContext()->getRenderableSet()->removeRenderable(towerBase->getID());
    towerBase->cleanUp(gameContext->getRenderContext());
    towerBase.reset();
    }
  if (towerTurret)
    {
    gameContext->getRenderContext()->getRenderableSet()->removeRenderable(towerTurret->getID());
    towerTurret->cleanUp(gameContext->getRenderContext());
    towerTurret.reset();
    }
  }

bool Tower::inflictDamage(int damagePoints)
  {
  showDamageEffect = true;
  healthPoints -= damagePoints;
  return healthPoints <= 0;
  }

void Tower::setPlayerNum(uint playerNum)
  {
  this->playerNum = playerNum;
  }

void Tower::setPosition(const Vector3D& pos)
  {
  position = pos;
  if (towerBase)
    towerBase->getTransform()->setTransformMatrix(mathernogl::matrixTranslate(pos));
  if (towerTurret)
    towerTurret->getTransform()->setTransformMatrix(mathernogl::matrixTranslate(pos));
  setTargetOffset(targetOffset);
  }

void Tower::setTargetOffset(const Vector3D& offset)
  {
  targetOffset = offset;
  targetPosition = position + targetOffset;
  }

void Tower::setMaxHealthPoints(int max)
  {
  maxHealthPoints = max;
  if (healthPoints > maxHealthPoints)
    healthPoints = maxHealthPoints;
  }

void Tower::setTurretRotation(const Vector3D& targetPos)
  {
  if (towerTurret)
    {
    Vector3D targetDirection = targetPos - position;
    targetDirection.y = 0;
    targetDirection.makeUniform();

    float angleBetween = mathernogl::ccwAngleBetween(Vector2D(0, -1), Vector2D(targetDirection.x, targetDirection.z));
    towerTurret->getTransform()->setIdentityMatrix();
    towerTurret->getTransform()->rotate(0, 1, 0, angleBetween);
    towerTurret->getTransform()->translate(position);
    }
  }
