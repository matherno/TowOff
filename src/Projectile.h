#pragma once

#include <RenderSystem/RenderSystem.h>
#include <GameSystem/GameSystem.h>
#include "TimeToLiveActor.h"

/*
*   
*/

class Projectile : public TimeToLiveActor
  {
  RenderablePtr renderable;
  float scale = 1;
  string meshFilePath;
  string textureFilePath;
  Vector3D position;

public:
  Projectile(uint id);
  virtual ~Projectile() {};

  Vector3D getPosition() const { return position; }
  void setPosition(const Vector3D& position) { this->position = position; }
  void setScale(float scale) { this->scale = scale; }
  void setMeshFilePath(const string& filePath) { meshFilePath = filePath; }
  void setTextureFilePath(const string& filePath) { textureFilePath = filePath; }

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

protected:
  void updateRenderable(Vector3D direction);
  static void doAreaEffectDamage(GameContext* gameContext, const Vector3D& position, float radius, uint maxDamage);
  };
typedef std::shared_ptr<Projectile> ProjectilePtr;


class MortarProjectile : public Projectile
  {
private:
  Vector3D velocity;
  Vector3D acceleration;
  float dragEffect = 0;
  float damageRadius = 4;
  uint maxDamageAmount = 10;

public:
  MortarProjectile(uint id) : Projectile(id) {}

  virtual void onUpdate(GameContext* gameContext) override;
  void setVelocity(const Vector3D& velocity) { this->velocity = velocity; }
  void setAcceleration(const Vector3D& acceleration) { this->acceleration = acceleration; }
  void setDragEffect(float dragEffect) { this->dragEffect = dragEffect; }
  void setDamageRadius(float radius) { damageRadius = radius; }
  void setDamageAmount(uint damage) { maxDamageAmount = damage; }
  };