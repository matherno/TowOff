//
// Created by matt on 27/03/17.
//


#include "ParticleEmitter.h"

ParticleEmitter::ParticleEmitter()
  {

  }

void ParticleEmitter::createParticle(const mathernogl::Vector3D& initPosOffset, const mathernogl::Vector3D& velocity, long timeAlive)
  {
  Particle* particle = new Particle();
  particle->position = initPosOffset;
  particle->velocity = velocity;
  particle->timeToLive = timeAlive;
  particles.push_back(particle);
  }

void ParticleEmitter::updateParticles(long deltaTime)
  {
  timeSinceStart += deltaTime;
  while (timeSinceStart - lastSpawnTime > timeBetweenSpawns)
    {
    if (isSpawning)
      createParticle({0, 0, 0}, randomGenerator.getNormal() * initVelocity, timeAlive);
    lastSpawnTime += timeBetweenSpawns;
    }

  std::vector<Particle*>::iterator iter = particles.begin();
  while (iter != particles.end())
    {
    Particle* particle = *iter;
    particle->timeToLive -= deltaTime;
    particle->velocity.y -= gravityAccel * deltaTime;
    particle->position += particle->velocity * (int) deltaTime;
    if (particle->timeToLive <= 0)
      iter = particles.erase(iter);
    else
      iter++;
    }
  }

float ParticleEmitter::getGravityAccel() const
  {
  return gravityAccel;
  }

void ParticleEmitter::setGravityAccel(float gravityAccel)
  {
  ParticleEmitter::gravityAccel = gravityAccel;
  }

long ParticleEmitter::getTimeBetweenSpawns() const
  {
  return timeBetweenSpawns;
  }

void ParticleEmitter::setTimeBetweenSpawns(long timeBetweenSpawns)
  {
  ParticleEmitter::timeBetweenSpawns = timeBetweenSpawns;
  }

long ParticleEmitter::getTimeAlive() const
  {
  return timeAlive;
  }

void ParticleEmitter::setTimeAlive(long timeAlive)
  {
  ParticleEmitter::timeAlive = timeAlive;
  }
