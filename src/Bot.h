#pragma once

#include "TowerTarget.h"
#include "Tower.h"

/*
*   
*/

class Bot : public TowerTarget
  {
private:
  const uint typeID;
  Vector2D velocity = Vector2D(0, -1); // (x, z)
  float movementSpeed = 5;
  uint hitDamage = 20;
  bool showDamageEffect = false;
  Vector3D centreOfMassOffset;
  Transform botTransform;

public:
  Bot(uint id, uint botType);
  void setMovementSpeed(float speed) { movementSpeed = speed; }
  void setHitDamagePoints(uint hitDamage) { this->hitDamage = hitDamage; }
  void setCentreOfMassOffset(const Vector3D& offset) { this->centreOfMassOffset = offset; }
  uint getType() const { return typeID; }

  Vector2D getVelocity2D() const { return velocity; }
  Vector3D getCentreOfMass() const { return getPosition() + centreOfMassOffset; }
  virtual Vector3D getTargetPosition() const override { return getCentreOfMass(); }
  const Transform* getTransform() const { return &botTransform; }

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