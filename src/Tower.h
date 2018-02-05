#pragma once

#include <GameSystem/GameSystem.h>

/*
*   
*/

class TowerWeapon;
class TowerFunctionality;
typedef std::unique_ptr<TowerFunctionality> TowerFunctionalityPtr;

class Tower : public GameActor
  {
public:
  enum TowerFunction
    {
    combat,
    storage,
    relay,
    miner,
    };

private:
  uint towerType;
  TowerFunctionalityPtr functionality;
  RenderablePtr towerBase;
  RenderablePtr towerTurret;
  Vector3D position;
  Vector3D targetPosition;
  Vector3D connectPosition;
  uint playerNum = 0;
  int maxHealthPoints = 100;
  int healthPoints = maxHealthPoints;
  bool showDamageEffect = false;

protected:
  float hitRadius = 5;
  float connectRadius = 10;
  string baseModelFile, turretModelFile = "";
  Vector3D targetOffset;
  Vector3D connectOffset;

public:
  Tower(uint id, uint towerType, TowerFunctionalityPtr functionality);
  virtual ~Tower() {};
  void setPlayerNum(uint playerNum);
  uint getPlayerNum() const { return playerNum; }
  TowerFunction getFunction() const;
  bool isPowerSrc() const { return getFunction() == miner || getFunction() == storage; }
  uint getTowerType() const { return towerType; }
  void setBaseModelFilePath(string filePath){ baseModelFile = filePath; }
  void setTurretModelFilePath(string filePath){ turretModelFile = filePath; }

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

  //  returns true if this caused the tower to be destroyed
  bool inflictDamage(int damagePoints);

  Vector3D getPosition() const { return position; }
  Vector3D getTargetPosition() const { return targetPosition; }
  Vector3D getConnectPosition() const { return connectPosition; }
  Vector3D getHitRadius() const { return hitRadius; }
  int getHealthPoints() const { return healthPoints; }
  void setPosition(const Vector3D& pos);
  void setTargetOffset(const Vector3D& offset);
  void setConnectOffset(const Vector3D& offset);
  void setMaxHealthPoints(int max);
  bool isAlive() { return healthPoints > 0; }
  void setConnectRadius(float radius) { connectRadius = radius; }
  float getConnectRadius() const { return connectRadius; }
  void setTurretRotation(const Vector3D& targetPos);
  const Transform* getTurretRotation() const;
  };

typedef std::shared_ptr<Tower> TowerPtr;
typedef mathernogl::MappedList<TowerPtr> TowerList;

class TowerWeapon
  {
private:
  std::weak_ptr<Tower> target;

public:
  TowerWeapon() {}
  virtual ~TowerWeapon() {}

  void setTarget(TowerPtr target) { this->target = target; }
  TowerPtr getTarget() { return target.lock(); }
  virtual void initShooting(GameContext* context) = 0;
  virtual void updateShooting(GameContext* context, const Vector3D& shootPos) = 0;
  virtual void endShooting(GameContext* context, const Vector3D& shootPos) = 0;
  virtual bool isCoolingDown(long currentTime) = 0;
  };

class TowerFunctionality
  {
public:
  const Tower::TowerFunction function;
  TowerFunctionality(Tower::TowerFunction function) : function(function) {}
  virtual void onAttached(Tower* tower, GameContext* gameContext) {};
  virtual void onUpdate(Tower* tower, GameContext* gameContext) {};
  virtual void onDetached(Tower* tower, GameContext* gameContext) {};
  };
typedef std::unique_ptr<TowerFunctionality> TowerFunctionalityPtr;