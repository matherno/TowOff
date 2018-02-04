#pragma once

class TOGameContext;

#include <GameSystem/GameSystem.h>
#include <GameSystem/GameContextImpl.h>
#include <ParticleSystem/ParticleSystem.h>
#include <RenderSystem/RenderableTerrain.h>
#include "Tower.h"
#include "Player.h"
#include "Projectile.h"
#include "TOInputHandler.h"
#include "HUDHandler.h"
#include "ConnectionManager.h"

/*
*   Sub-class of Game Context to capture the central state of the TowOff game
*/

class TOGameContext : public GameContextImpl
  {
private:
  std::vector<PlayerPtr> players;
  TowerList towers;
  std::shared_ptr<RenderableTerrain> surfaceMesh;
  ParticleSystemPtr towerDamageParticles;
  std::shared_ptr<TOInputHandler> inputHandler;
  HUDHandler hudHandler;
  std::shared_ptr<ConnectionManager> connectionManager;

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
  uint getActivePlayer() const;
  void setActivePlayer(uint player);
  HUDHandler* getHUDHandler(){ return &hudHandler; }

  TowerPtr getTower(uint id);
  TowerPtr getClosestTowerTo(const Tower* tower, bool onlyEnemies);
  int numTowers() const { return towers.count(); }
  void removeTower(uint id);
  bool isConnectedToPowerSrc(uint towerID) const;

  TowerPtr createTower(uint towerType, const Vector3D& position = Vector3D(0));

  void doTowerDamageEffect(const Tower* tower);
  Vector3D terrainHitTest(uint cursorX, uint cursorY, bool* isLand = nullptr) const;
  bool isPositionLand(const Vector3D& position) const;

  inline static TOGameContext* cast(GameContext* context)
    {
    TOGameContext* toContext = dynamic_cast<TOGameContext*>(context);
    ASSERT(toContext, "Given game context wasn't a TOGameContext!");
    return toContext;
    }

protected:
  void initSurface(uint size);
  void initDamageParticleSystem();
  void initUI();
  };
