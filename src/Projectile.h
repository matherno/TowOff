#pragma once

#include <src/RenderSystem/RenderSystem.h>
#include <src/GameSystem/GameSystem.h>

/*
*   
*/

class Projectile : public GameActor
  {
private:
  Vector3D position;
  Vector3D velocity;
  Vector3D acceleration;
  float dragEffect = 0;
  long timeToLive = 2000;
  RenderablePtr renderable;
  float scale = 1;
  string meshFilePath;

public:
  Projectile(uint id);
  virtual ~Projectile() {};

  void setPosition(const Vector3D& position) { this->position = position; }
  void setVelocity(const Vector3D& velocity) { this->velocity = velocity; }
  void setAcceleration(const Vector3D& acceleration) { this->acceleration = acceleration; }
  void setDragEffect(float dragEffect) { this->dragEffect = dragEffect; }
  void setTimeToLive(long timeToLive) { this->timeToLive = timeToLive; }
  void setScale(float scale) { this->scale = scale; }
  void setMeshFilePath(const string& filePath) { meshFilePath = filePath; }

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

  };
typedef std::shared_ptr<Projectile> ProjectilePtr;