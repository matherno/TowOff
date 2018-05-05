#pragma once

#include <RenderSystem/RenderableMesh.h>
#include "TowerTarget.h"
#include "Tower.h"

/*
*   
*/

class Bot : public TowerTarget
  {
private:
  const uint typeID;
  const string meshFilePath;
  std::shared_ptr<RenderableMesh> renderable;
  Vector2D velocity = Vector2D(0, -1); // (x, z)
  float movementSpeed = 5;
  uint hitDamage = 20;
  bool showDamageEffect = false;

public:
  Bot(uint id, uint botType, const string& meshFilePath);
  void setMovementSpeed(float speed) { movementSpeed = speed; }
  void setHitDamagePoints(uint hitDamage) { this->hitDamage = hitDamage; }

  Vector2D getVelocity2D() const { return velocity; }

  virtual bool doDamage(uint damagePoints) override;
  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

protected:
  void doSwarmMovement(GameContext* gameContext);
  void checkTowerCollision(GameContext* gameContext, TowerPtr tower);
  void destroyThis(GameContext* gameContext);
  };

typedef std::shared_ptr<Bot> BotPtr;
typedef mathernogl::MappedList<BotPtr> BotList;