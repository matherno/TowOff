#pragma once

#include <src/GameSystem/GameSystem.h>
#include "ParticleEmitter.h"

/*
*   
*/

struct ParticleSystemItem
  {
  uint id;
  ParticleEmitterPtr baseEmitter;
  RenderablePtr renderable;
  long timeToLive;
  Vector3D position;
  Vector3D colour;
  };

class ParticleSystem : public GameActor
  {
private:
  mathernogl::MappedList<ParticleSystemItem> emitters;
  float gravityAccel = 0.001;
  long timeBetweenSpawns = 1000;
  long timeAlive = 1000;
  float size = 3.0f;
  float initVelocity = 0.05f;
  uint nextEmitterID = 0;

public:
  ParticleSystem(uint id);

  void addEmitter(const Vector3D& position, long timeToLive, const Vector3D& colour);
  void setGravityAccel(float gravityAccel);
  void setTimeBetweenSpawns(long timeBetweenSpawns);
  void setTimeAlive(long timeAlive);
  void setParticleSize(float size);
  void setInitVelocity(float velocity);

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

protected:
  void updateEmittersParams();
  void initNewRenderables(RenderContext* renderContext);
  };

typedef std::shared_ptr<ParticleSystem> ParticleSystemPtr;
