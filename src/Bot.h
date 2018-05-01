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
  std::unique_ptr<Vector3D> targetPosition;
  std::weak_ptr<Tower> targetTower;
  float movementSpeed = 0.5;
  uint hitDamage = 20;
  bool showDamageEffect = false;

public:
  Bot(uint id, uint botType, const string& meshFilePath);
  void moveToPosition(const Vector3D& position);
  void setMovementSpeed(float speed) { movementSpeed = speed; }
  void setHitDamagePoints(uint hitDamage) { this->hitDamage = hitDamage; }

  virtual bool doDamage(uint damagePoints) override;
  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

protected:
  void doMovement(GameContext* gameContext);
  void findTarget(GameContext* gameContext);
  void checkTowerCollision(GameContext* gameContext);
  TowerPtr getTargetTower();
  void destroyThis(GameContext* gameContext);
  };

typedef std::shared_ptr<Bot> BotPtr;
typedef mathernogl::MappedList<BotPtr> BotList;