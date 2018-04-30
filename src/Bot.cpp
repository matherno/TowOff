//
// Created by matt on 29/04/18.
//

#include "Bot.h"
#include "Resources.h"

Bot::Bot(uint id, uint botType, const string& meshFilePath) : TowerTarget(id), typeID(botType), meshFilePath(meshFilePath)
  {}

void Bot::onAttached(GameContext* gameContext)
  {
  TowerTarget::onAttached(gameContext);

  RenderContext* renderContext = gameContext->getRenderContext();
  renderable.reset(new RenderableMesh(renderContext->getNextRenderableID()));
  renderable->setMeshStorage(renderContext->getSharedMeshStorage(meshFilePath));
  renderable->setDrawStyleTexture(renderContext->getSharedTexture(IMAGE_TEXTURE_PALETTE));
  renderable->getTransform()->setTransformMatrix(mathernogl::matrixTranslate(getPosition()));
  renderable->initialise(renderContext);
  renderContext->getRenderableSet()->addRenderable(renderable);
  }

void Bot::onUpdate(GameContext* gameContext)
  {
  TowerTarget::onUpdate(gameContext);

  // temporary circular movement
  Vector3D newTargetPos = getPosition() + Vector3D(-200, 0, 0);
  if (targetPosition)
    {
    newTargetPos = mathernogl::crossProduct(Vector3D(0, 1, 0), (*targetPosition - getPosition()).getUniform()).getUniform();
    newTargetPos += *targetPosition;
    }
  moveToPosition(newTargetPos);

  doMovement(gameContext);
  }

void Bot::onDetached(GameContext* gameContext)
  {
  TowerTarget::onDetached(gameContext);

  RenderContext* renderContext = gameContext->getRenderContext();
  renderable->cleanUp(renderContext);
  renderContext->getRenderableSet()->removeRenderable(renderable->getID());
  renderable.reset();
  }

void Bot::moveToPosition(const Vector3D& position)
  {
  targetPosition.reset(new Vector3D(position));
  }

void Bot::doMovement(GameContext* gameContext)
  {
  if (targetPosition)
    {
    //  update position
    const Vector3D posToTarget = *targetPosition - getPosition();
    float distance = movementSpeed * gameContext->getDeltaTime() * 0.01f;
    if (distance <= posToTarget.magnitude())
      {
      // haven't reached the target yet
      setPosition(getPosition() + (posToTarget.getUniform() * distance));
      }
    else
      {
      // reached the target
      setPosition(*targetPosition);
      targetPosition.reset();
      }

    //  update orientation
    Vector3D targetDirection = posToTarget;
    targetDirection.y = 0;
    targetDirection.makeUniform();
    double angleBetween = mathernogl::ccwAngleBetween(Vector2D(0, -1), Vector2D(targetDirection.x, targetDirection.z));

    //  update renderable transform
    mathernogl::Matrix4 transform = mathernogl::matrixRotate(Vector3D(0, 1, 0), angleBetween) * mathernogl::matrixTranslate(getPosition());
    renderable->getTransform()->setTransformMatrix(transform);
    }
  }
