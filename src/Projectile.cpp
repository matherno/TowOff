//
// Created by matt on 13/12/17.
//

#include <RenderSystem/RenderableMesh.h>
#include "Projectile.h"
#include "Resources.h"
#include "TOGameContext.h"

/*
 * Projectile
 */

Projectile::Projectile(uint id) : TimeToLiveActor(id)
  {}

void Projectile::onAttached(GameContext* gameContext)
  {
  renderable.reset();
  if (!meshFilePath.empty())
    {
    RenderContext* renderContext = gameContext->getRenderContext();
    RenderableMesh* mesh = new RenderableMesh(renderContext->getNextRenderableID());
    mesh->setMeshStorage(renderContext->getSharedMeshStorage(meshFilePath));
    if (!textureFilePath.empty())
      mesh->setDrawStyleTexture(renderContext->getSharedTexture(textureFilePath));
    renderable.reset(mesh);
    renderable->initialise(renderContext);
    renderContext->getRenderableSet()->addRenderable(renderable);
    renderable->getTransform()->scale(scale);
    renderable->getTransform()->translate(position);
    }
  }

void Projectile::onUpdate(GameContext* gameContext)
  {
  TimeToLiveActor::onUpdate(gameContext);
  }

void Projectile::onDetached(GameContext* gameContext)
  {
  if (renderable)
    {
    gameContext->getRenderContext()->getRenderableSet()->removeRenderable(renderable->getID());
    renderable->cleanUp(gameContext->getRenderContext());
    }
  }

void Projectile::updateRenderable(Vector3D direction)
  {
  renderable->getTransform()->setIdentityMatrix();
  renderable->getTransform()->scale(scale);
  renderable->getTransform()->rotateBetween(Vector3D(0, 0, -1), direction);
  renderable->getTransform()->translate(position);
  }

void Projectile::doAreaEffectDamage(GameContext* gameContext, const Vector3D& position, float radius, uint maxDamage)
  {
  TOGameContext::cast(gameContext)->forEachTowerTarget ([position, radius, maxDamage](TowerTargetPtr towerTarget)
    {
    const float distance = (float) position.distanceToPoint(towerTarget->getTargetPosition());
    float damage = 1 - (distance / radius);
    if (damage > 0)
      towerTarget->doDamage((uint)std::pow(damage * maxDamage, 2));
    });
  }


/*
 * MortarProjectile
 */

void MortarProjectile::onUpdate(GameContext* gameContext)
  {
  Projectile::onUpdate(gameContext);

  float deltaTimeSeconds = (float)gameContext->getDeltaTime() / 1000.0f;
  velocity += acceleration * deltaTimeSeconds;
  velocity -= velocity * dragEffect * deltaTimeSeconds;
  setPosition(getPosition() + velocity * deltaTimeSeconds);
  updateRenderable(velocity.getUniform());

  if (getPosition().y <= 0)
    {
    doAreaEffectDamage(gameContext, getPosition(), damageRadius, maxDamageAmount);
    TOGameContext::cast(gameContext)->doWeaponExplosionEffect(getPosition(), 6);
    gameContext->removeActor(getID());
    }
  }


/*
 * TrackingMissileProjectile
 */

void TrackingMissileProjectile::onUpdate(GameContext* gameContext)
  {
  Projectile::onUpdate(gameContext);

  const Vector3D targetPos = getTargetPosition();
  const float displacement = (float)gameContext->getDeltaTime() * 0.01f * missileSpeed;
  Vector3D posToTarget = targetPos - getPosition();
  if (posToTarget.magnitude() > displacement)
    {
    posToTarget.makeUniform();
    setPosition(getPosition() + posToTarget * displacement);
    updateRenderable(posToTarget);
    }
  else
    {
    setPosition(targetPos);
    doAreaEffectDamage(gameContext, getPosition(), damageRadius, maxDamageAmount);
    TOGameContext::cast(gameContext)->doWeaponExplosionEffect(getPosition(), 2);
    gameContext->removeActor(getID());
    }
  }
