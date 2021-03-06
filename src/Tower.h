#pragma once

#include <GameSystem/GameSystem.h>
#include "TOGameSaveLoad.h"
#include "TowerTarget.h"

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
    none,
    combat,
    storage,
    relay,
    miner,
    homebase,
    };

protected:
  const uint towerType;
  TowerFunctionalityPtr functionality;
  RenderablePtr towerBase;
  RenderablePtr towerTurret;
  Vector3D position;
  Vector3D targetPosition;
  Vector3D connectPosition;
  int maxHealthPoints = 100;
  int healthPoints = maxHealthPoints;
  bool showDamageEffect = false;
  uint storedEnergy = 0;
  uint maxEnergy = 500;
  float hitRadius = 5;
  float connectRadius = 10;
  float visibilityRadius = 30;
  string baseModelFile, turretModelFile = "";
  Vector3D targetOffset;
  Vector3D connectOffset;
  double turretRotation = 0;

public:
  Tower(uint id, uint towerType, TowerFunctionalityPtr functionality);
  virtual ~Tower() {};

  virtual TowerFunction getFunction() const;
  bool isPowerSrc() const { return getFunction() == miner || getFunction() == storage || getFunction() == homebase; }
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
  float getHitRadius() const { return hitRadius; }
  int getHealthPoints() const { return healthPoints; }
  int getMaxHealthPoints() const { return maxHealthPoints; }
  float getHealthFactor() const { return (float)healthPoints / (float)maxHealthPoints; }
  virtual bool isUnderConstruction() const{ return false; }

  bool hasEnergy() const { return storedEnergy > 0; }
  uint getStoredEnergy() const { return storedEnergy; }
  uint getMaxEnergy() const { return maxEnergy; }
  void setMaxEnergy(uint max) { maxEnergy = max; }
  uint getAvailableEnergyStorage() const { return maxEnergy - storedEnergy; }

  // returns the amount taken/stored
  uint takeEnergy(uint amount, bool allOrNothing);
  uint storeEnergy(uint amount);

  void setPosition(const Vector3D& pos);
  void setTargetOffset(const Vector3D& offset);
  void setConnectOffset(const Vector3D& offset);
  void setMaxHealthPoints(int max);
  void setHitRadius(float radius) { hitRadius = radius; }
  bool isAlive() { return healthPoints > 0; }
  void setConnectRadius(float radius) { connectRadius = radius; }
  float getConnectRadius() const { return connectRadius; }
  void setVisibilityRadius(float radius) { visibilityRadius = radius; }
  float getVisibilityRadius() const { return visibilityRadius; }
  void setTurretRotation(const Vector3D& targetPos);
  const Transform* getTurretTransform() const;
  void setTurretYRotation(double yRotation);
  double getTurretYRotation() const;
  RenderablePtr getBaseMeshRenderable() { return towerBase; }
  RenderablePtr getTurretMeshRenderable() { return towerTurret; }

  void getTowerState(TowerState* state);
  void setTowerState(const TowerState* state);
  };

typedef std::shared_ptr<Tower> TowerPtr;
typedef mathernogl::MappedList<TowerPtr> TowerList;

class TowerWeapon
  {
private:
  std::weak_ptr<TowerTarget> target;

public:
  TowerWeapon() {}
  virtual ~TowerWeapon() {}

  void setTarget(TowerTargetPtr target) { this->target = target; }
  TowerTargetPtr getTarget() { return target.lock(); }
  virtual void initShooting(GameContext* context, Tower* sourceTower) = 0;
  virtual void updateIdle(GameContext* context, Tower* sourceTower) {};

  //  returns false when shooting should stop after a call to this
  virtual bool updateShooting(GameContext* context, Tower* sourceTower, const Vector3D& shootPos) = 0;
  virtual void endShooting(GameContext* context, Tower* sourceTower) = 0;
  virtual bool isCoolingDown() = 0;
  };

class TowerFunctionality
  {
public:
  const Tower::TowerFunction function;
  TowerFunctionality(Tower::TowerFunction function) : function(function) {}
  virtual void onAttached(Tower* tower, GameContext* gameContext) {};
  virtual void onUpdate(Tower* tower, GameContext* gameContext) {};
  virtual void onDetached(Tower* tower, GameContext* gameContext) {};
  virtual void onGetTowerState(TowerState* state) {};
  virtual void onSetTowerState(const TowerState* state) {};
  };
typedef std::unique_ptr<TowerFunctionality> TowerFunctionalityPtr;