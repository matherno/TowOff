#pragma once

class TOGameContext;

#include <src/GameSystem/GameSystem.h>
#include <src/GameSystem/GameContextImpl.h>
#include <src/ParticleSystem/ParticleSystem.h>
#include "Tower.h"
#include "Player.h"
#include "Projectile.h"

/*
*   Sub-class of Game Context to capture the central state of the TowOff game
*/

class TOGameContext : public GameContextImpl
  {
private:
  std::vector<PlayerPtr> players;
  TowerList towers;
  RenderablePtr surfaceMesh;
  ParticleSystemPtr towerDamageParticles;

public:
  virtual bool initialise() override;
  virtual void cleanUp() override;
  virtual void processInputStage() override;
  virtual void processUpdateStage() override;
  virtual void processDrawStage() override;

  Player* getPlayer(uint num);
  Player* createPlayer();
  int numPlayers() const { return (int)players.size(); }
  Vector3D getPlayerColour(uint num) const;

  TowerPtr getTower(uint id);
  TowerPtr getClosestTowerTo(const Tower* tower, bool onlyEnemies);
  int numTowers() const { return towers.count(); }
  void removeTower(uint id);

  TowerPtr createBasicTower(const Vector3D& position = Vector3D(0));
  TowerPtr createBasicTowerProj(const Vector3D& position = Vector3D(0));

  ProjectilePtr createFootballProjectile(uint id);

  void doTowerDamageEffect(const Tower* tower);
  Vector3D terrainHitTest(uint cursorX, uint cursorY);

  inline static TOGameContext* cast(GameContext* context)
    {
    TOGameContext* toContext = dynamic_cast<TOGameContext*>(context);
    ASSERT(toContext, "Given game context wasn't a TOGameContext!");
    return toContext;
    }

protected:
  void initSurface(uint numCells, float cellSize);
  void initDamageParticleSystem();
  };
