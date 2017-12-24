//
// Created by matt on 24/12/17.
//

#include "ParticleSystem.h"
#include "RenderableParticles.h"

ParticleSystem::ParticleSystem(uint id) : GameActor(id)
  {}

void ParticleSystem::onAttached(GameContext* gameContext)
  {

  }

void ParticleSystem::onUpdate(GameContext* gameContext)
  {
  initNewRenderables(gameContext->getRenderContext());
  std::list<uint> emittersToRemove;
  for (ParticleSystemItem& item : *emitters.getList())
    {
    ParticleEmitterPtr& emitter = item.baseEmitter;
    emitter->updateParticles(gameContext->getDeltaTime());
    item.timeToLive -= gameContext->getDeltaTime();
    if (item.timeToLive <= 0)
      {
      emittersToRemove.emplace_back(item.id);
      gameContext->getRenderContext()->getRenderableSet()->removeRenderable(item.renderable->getID());
      item.renderable->cleanUp(gameContext->getRenderContext());
      item.renderable.reset();
      }
    else if (item.timeToLive <= timeAlive)
      {
      emitter->setSpawningState(false);
      }
    }

  for (uint id : emittersToRemove)
    emitters.remove(id);
  }

void ParticleSystem::onDetached(GameContext* gameContext)
  {
  for (ParticleSystemItem& item : *emitters.getList())
    {
    if (item.renderable)
      {
      gameContext->getRenderContext()->getRenderableSet()->removeRenderable(item.renderable->getID());
      item.renderable->cleanUp(gameContext->getRenderContext());
      }
    }
  emitters.clear();
  }

void ParticleSystem::setGravityAccel(float gravityAccel)
  {
  this->gravityAccel = gravityAccel;
  updateEmittersParams();
  }

void ParticleSystem::setTimeBetweenSpawns(long timeBetweenSpawns)
  {
  this->timeBetweenSpawns = timeBetweenSpawns;
  updateEmittersParams();
  }

void ParticleSystem::setTimeAlive(long timeAlive)
  {
  this->timeAlive = timeAlive;
  updateEmittersParams();
  }

void ParticleSystem::setParticleSize(float size)
  {
  if (size > 1)
    this->size = size;
  else
    this->size = 1;
  updateEmittersParams();
  }

void ParticleSystem::setInitVelocity(float velocity)
  {
  this->initVelocity = velocity;
  updateEmittersParams();
  }

void ParticleSystem::addEmitter(const Vector3D& position, long timeToLive, const Vector3D& colour)
  {
  ParticleEmitterPtr emitter(new ParticleEmitter());
  ParticleSystemItem item;
  item.id = nextEmitterID++;
  item.timeToLive = timeToLive + timeAlive;   // allows for extra time for particles to die off
  item.baseEmitter = emitter;
  item.position = position;
  item.colour = colour;
  emitters.add(item, item.id);
  updateEmittersParams();
  }

void ParticleSystem::updateEmittersParams()
  {
  for (ParticleSystemItem& item : *emitters.getList())
    {
    ParticleEmitterPtr& emitter = item.baseEmitter;
    emitter->setTimeAlive(timeAlive);
    emitter->setTimeBetweenSpawns(timeBetweenSpawns);
    emitter->setInitVelocity(initVelocity);
    emitter->setGravityAccel(gravityAccel);
    emitter->setParticleSize(size);
    }
  }

void ParticleSystem::initNewRenderables(RenderContext* renderContext)
  {
  for (ParticleSystemItem& item : *emitters.getList())
    {
    if (!item.renderable)
      {
      RenderableParticles* particles = new RenderableParticles(renderContext->getNextRenderableID());
      particles->setEmitter(item.baseEmitter);
      particles->setTransform(mathernogl::matrixTranslate(item.position));
      particles->setColour(item.colour);
      particles->initialise(renderContext);
      item.renderable.reset(particles);
      renderContext->getRenderableSet()->addRenderable(item.renderable);
      }
    }
  }
