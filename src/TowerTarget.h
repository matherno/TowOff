#pragma once

#include <GameSystem/GameSystem.h>

/*
*   
*/

class TowerTarget : public GameActor
  {
private:
  Vector3D position;
  float hitRadius = 1;
  uint maxHealthPoints = 20;
  uint healthPoints = maxHealthPoints;

public:
  TowerTarget(uint id);
  virtual ~TowerTarget() {}
  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

  void setPosition(const Vector3D& position) { this->position = position; }
  void setHitRadius(float radius) { this->hitRadius = radius; }
  void setHealthPoints(uint health) { this->healthPoints = health; }
  void setMaxHealthPoints(uint max);

  Vector3D getPosition() const { return position; }
  virtual Vector3D getTargetPosition() const { return getPosition(); }
  Vector2D getPosition2D() const;
  float getHitRadius() const { return hitRadius; }
  uint getHealthPoints() const { return healthPoints; }
  uint getMaxHealthPoints() const { return maxHealthPoints; }
  bool isAlive() const { return healthPoints > 0; }
  bool isInRange(const Vector3D& rangeCentre, float maxRange, float minRange) const;

  //  returns true if this targets health is all gone
  virtual bool doDamage(uint damagePoints);
  };

typedef std::shared_ptr<TowerTarget> TowerTargetPtr;