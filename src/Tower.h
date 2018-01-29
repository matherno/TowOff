#pragma once

#include <GameSystem/GameSystem.h>

/*
*   
*/

class TowerWeapon;

class Tower : public GameActor
  {
public:
  enum TowerState
    {
    idle,
    shooting,
    };

private:
  RenderablePtr towerBase;
  RenderablePtr towerTurret;
  Vector3D position;
  Vector3D targetPosition;
  uint playerNum = 0;
  TowerState state = idle;
  int maxHealthPoints = 100;
  int healthPoints = maxHealthPoints;
  bool showDamageEffect = false;

protected:
  float hitRadius = 5;
  std::unique_ptr<TowerWeapon> weapon;
  string baseModelFile, turretModelFile;
  Vector3D shootOffset;
  Vector3D targetOffset;

public:
  Tower(uint id, string baseModelFile, string turretModelFile);
  virtual ~Tower() {};
  void setPlayerNum(uint playerNum);
  uint getPlayerNum() const { return playerNum; }
  TowerState getState(){ return state; }

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

  //  returns true if this caused the tower to be destroyed
  bool inflictDamage(int damagePoints);

  Vector3D getPosition() const { return position; }
  Vector3D getTargetPosition() const { return targetPosition; }
  Vector3D getHitRadius() const { return hitRadius; }
  int getHealthPoints() const { return healthPoints; }
  void setPosition(const Vector3D& pos);
  void setShootOffset(const Vector3D& offset) { shootOffset = offset; }
  void setTargetOffset(const Vector3D& offset);
  void setWeapon(std::unique_ptr<TowerWeapon> weapon) { this->weapon = std::move(weapon); }
  void setMaxHealthPoints(int max);
  bool isAlive() { return healthPoints > 0; }

protected:
  void setTurretRotation(const Vector3D& targetPos);
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