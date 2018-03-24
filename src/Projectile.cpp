//
// Created by matt on 13/12/17.
//

#include <RenderSystem/RenderableMesh.h>
#include "Projectile.h"
#include "Resources.h"

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
  float deltaTimeSeconds = (float)gameContext->getDeltaTime() / 1000.0f;
  velocity += acceleration * deltaTimeSeconds;
  velocity -= velocity * dragEffect * deltaTimeSeconds;
  position += velocity * deltaTimeSeconds;
  renderable->getTransform()->setIdentityMatrix();
  renderable->getTransform()->scale(scale);
  renderable->getTransform()->rotateBetween(Vector3D(0, 0, -1), velocity.getUniform());
  renderable->getTransform()->translate(position);
  }

void Projectile::onDetached(GameContext* gameContext)
  {
  if (renderable)
    {
    gameContext->getRenderContext()->getRenderableSet()->removeRenderable(renderable->getID());
    renderable->cleanUp(gameContext->getRenderContext());
    }
  }
