//
// Created by matt on 29/04/18.
//

#include "Bot.h"
#include "Resources.h"
#include "TOGameContext.h"

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
  if (showDamageEffect)
    {
    TOGameContext::cast(gameContext)->doBotDamageEffect(this);
    showDamageEffect = false;
    }

  if (!isAlive())
    {
    destroyThis(gameContext);
    return;
    }

  TowerTarget::onUpdate(gameContext);
  doSwarmMovement(gameContext);
  }

void Bot::onDetached(GameContext* gameContext)
  {
  TowerTarget::onDetached(gameContext);

  RenderContext* renderContext = gameContext->getRenderContext();
  renderable->cleanUp(renderContext);
  renderContext->getRenderableSet()->removeRenderable(renderable->getID());
  renderable.reset();
  }

bool Bot::doDamage(uint damagePoints)
  {
  if (damagePoints > 0)
    showDamageEffect = true;
  return TowerTarget::doDamage(damagePoints);
  }

#define NEIGHBOUR_RADIUS 10
#define SEPARATION_RADIUS 7
#define TOWER_ATTACK_RADIUS 7

void Bot::doSwarmMovement(GameContext* gameContext)
  {
  TOGameContext* toGameContext = TOGameContext::cast(gameContext);
  const BotList* botList = toGameContext->getBotList();
  const BotPortalList* botPortalList = toGameContext->getBotPortalList();

  //  get swarm vectors based on neighbour bots => alignment, cohesion, separation
  const Vector2D position = getPosition2D();
  const Vector2D previousVelocity = getVelocity2D();
  Vector2D alignment, cohesion, separation;
  Vector2D neighbourPos, thisToNeighbour;
  uint numNeighbours = 0;
  const float hitRadius = getHitRadius();
  for (const BotPtr& bot : *botList->getList())
    {
    if (bot->getID() == getID())
      continue;

    neighbourPos = bot->getPosition2D();
    thisToNeighbour = neighbourPos - position;
    const float distance = thisToNeighbour.magnitude() - hitRadius - bot->getHitRadius();
    if (distance < NEIGHBOUR_RADIUS)
      {
      ++numNeighbours;
      alignment += bot->getVelocity2D();
      cohesion += neighbourPos;
      if (distance < SEPARATION_RADIUS)
        {
        thisToNeighbour.makeUniform();
        thisToNeighbour *= 1.0f - (distance / SEPARATION_RADIUS);
        separation -= thisToNeighbour;
        }
      }
    }

  //  factor neighbouring bot portals into the separation vector
  for (const BotPortalPtr& portal : *botPortalList->getList())
    {
    thisToNeighbour = portal->getPosition2D() - position;
    const float distance = thisToNeighbour.magnitude() - hitRadius - portal->getHitRadius();
    if (distance < SEPARATION_RADIUS)
      {
      thisToNeighbour.makeUniform();
      thisToNeighbour *= 1.0f - (distance / SEPARATION_RADIUS);
      thisToNeighbour *= 4.0f;
      separation -= thisToNeighbour;
      }
    }

  //  add those swarm vectors to velocity
  if (numNeighbours > 0)
    {
    alignment.x /= numNeighbours;
    alignment.y /= numNeighbours;
    alignment = (alignment - velocity) / 8;
    cohesion.x /= numNeighbours;
    cohesion.y /= numNeighbours;
    cohesion = cohesion - position;
    }
  velocity += alignment + cohesion + separation;

  //  find closest tower and make bot head roughly in its direction
  TowerPtr targetTower = toGameContext->findClosestTower(getPosition());
  if (targetTower)
    {
    const Vector2D towerPos(targetTower->getPosition().x, targetTower->getPosition().z);
    const Vector2D posToTower = towerPos - position;
    float multiplier = 1;
    if (numNeighbours == 0)
      multiplier = movementSpeed;
    else if (posToTower.magnitude() < TOWER_ATTACK_RADIUS)
      multiplier = 5;
    velocity += posToTower.getUniform() * multiplier;
    }

  //  limit the speed
  velocity = velocity * 0.1f + previousVelocity * 0.9f;
  if (velocity.magnitude() > movementSpeed)
    velocity = velocity.getUniform() * movementSpeed;

  // update position, checking if leaving map
  Vector2D deltaPos = velocity * (float)gameContext->getDeltaTime() * 0.001f;
  Vector2D newPosition = position + deltaPos;
  if (!toGameContext->isPositionOnMap(Vector3D(newPosition.x, 0, newPosition.y)))
    {
    velocity *= -1;
    newPosition = position - deltaPos;
    }
  setPosition(Vector3D(newPosition.x, 0, newPosition.y));

  //  update the renderables transform
  double angleBetween = mathernogl::ccwAngleBetween(Vector2D(0, -1), velocity.getUniform());
  mathernogl::Matrix4 transform = mathernogl::matrixRotate(Vector3D(0, 1, 0), angleBetween) * mathernogl::matrixTranslate(getPosition());
  renderable->getTransform()->setTransformMatrix(transform);

  //  check if colliding with closest tower, and blow up if so
  if (targetTower)
    checkTowerCollision(gameContext, targetTower);
  }

void Bot::checkTowerCollision(GameContext* gameContext, TowerPtr tower)
  {
  if (!tower->isAlive())
    {
    tower.reset();
    return;
    }

  const float botHitRadius = getHitRadius();
  const float towerHitRadius = tower->getHitRadius();
  if (getPosition().distanceToPoint(tower->getPosition()) < botHitRadius + towerHitRadius)
    {
    tower->inflictDamage(hitDamage);
    destroyThis(gameContext);
    }
  }

void Bot::destroyThis(GameContext* gameContext)
  {
  setHealthPoints(0);
  TOGameContext::cast(gameContext)->removeBot(getID());
  }
