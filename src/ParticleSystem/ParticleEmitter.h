#pragma once
//
// Created by matt on 27/03/17.
//

#include <algorithm>
#include <list>

#include <mathernogl/MathernoGL.h>

struct Particle
  {
  mathernogl::Vector3D position;
  mathernogl::Vector3D velocity;
  long timeToLive;
  };

class ParticleEmitter
  {
private:
  std::vector<Particle*> particles;
  mathernogl::RandomGenerator randomGenerator;
  long timeSinceStart = 0;
  long lastSpawnTime = 0;
  float gravityAccel = 0.001;
  long timeBetweenSpawns = 1000;
  long timeAlive = 10000;
  float size = 3.0f;
  float initVelocity = 0.05f;
  bool isSpawning = true;

public:
  ParticleEmitter();

  float getGravityAccel() const;
  void setGravityAccel(float gravityAccel);
  long getTimeBetweenSpawns() const;
  void setTimeBetweenSpawns(long timeBetweenSpawns);
  long getTimeAlive() const;
  void setTimeAlive(long timeAlive);
  void createParticle(const mathernogl::Vector3D& initPosOffset, const mathernogl::Vector3D& velocity, long timeAlive);
  void updateParticles(long deltaTime);
  void setParticleSize(float size){ this->size = size; }
  float getParticleSize() const { return size; }
  void setInitVelocity(float velocity){ initVelocity = velocity; }
  int count() { return (int)particles.size(); }
  void setSpawningState(bool isSpawning){ this->isSpawning = isSpawning; }

  const std::vector<Particle*>::const_iterator begin() const { return particles.cbegin(); }
  const std::vector<Particle*>::const_iterator end() const { return particles.cend(); }
  };

typedef std::shared_ptr<ParticleEmitter> ParticleEmitterPtr;


