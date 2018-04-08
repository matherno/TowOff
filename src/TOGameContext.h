#pragma once

class TOGameContext;

#include <GameSystem/GameSystem.h>
#include <GameSystem/GameContextImpl.h>
#include <ParticleSystem/ParticleSystem.h>
#include <RenderSystem/RenderableTerrain.h>
#include <GameSystem/Timer.h>
#include "Tower.h"
#include "Player.h"
#include "Projectile.h"
#include "TOInputHandler.h"
#include "HUDHandler.h"
#include "ConnectionManager.h"
#include "RangeFieldManager.h"
#include "SpecialEffectsHandler.h"
#include "TOGameSaveLoad.h"
#include "PauseMenuHandler.h"
#include "TowerSelectionManager.h"

/*
*   Sub-class of Game Context to capture the central state of the TowOff game
*/

class TOGameContext : public GameContextImpl
  {
private:
  std::shared_ptr<TOGameState> loadedGameState;
  std::vector<PlayerPtr> players;
  TowerList towers;
  std::shared_ptr<RenderableTerrain> surfaceMesh;
  HUDHandler hudHandler;
  SpecialEffectsHandler specialEffectsHandler;
  std::shared_ptr<ConnectionManager> connectionManager;
  std::shared_ptr<RangeFieldManager> rangeFieldManager;
  std::shared_ptr<TowerSelectionManager> selectionManager;
  std::shared_ptr<TOInputHandler> toInputHandler;
  std::shared_ptr<PauseMenuHandler> pauseMenuHandler;

  //  maps combat towers to the networks (via relay towers) that they are within range of
  std::map<uint, std::set<uint>> combatTowerNetworks;

  //  maps tower ids to a list of their bounding box ids
  std::map<uint, std::list<uint>> towerBoundingBoxes;

  //  maps tower ids to their displayed range field
  std::map<uint, uint> towerRangeFields;

public:
  TOGameContext(const RenderContextPtr& renderContext, std::shared_ptr<TOGameState> loadedGameState = nullptr) : GameContextImpl(renderContext), loadedGameState(loadedGameState) {}
  virtual bool initialise() override;
  virtual void cleanUp() override;
  virtual void processInputStage() override;
  virtual void processUpdateStage() override;
  virtual void processDrawStage() override;

  virtual FontPtr getDefaultFont() override;

  Player* getPlayer(uint num);
  Player* createPlayer();
  int numPlayers() const { return (int)players.size(); }
  Vector3D getPlayerColour(uint num) const;
  HUDHandler* getHUDHandler(){ return &hudHandler; }
  TowerSelectionManager* getSelectionManager(){ return selectionManager.get(); }
  void displayPauseMenu();

  TowerPtr getTower(uint id);
  const TowerList* getTowers() const { return &towers; }
  void forEachTower(std::function<void(TowerPtr tower)> func);
  TowerPtr getClosestTowerTo(const Tower* tower, bool onlyEnemies);
  int numTowers() const { return towers.count(); }
  void removeTower(uint id);
  TowerPtr createTower(uint towerType, const Vector3D& position, bool underConstruction);
  void addInitialTowers();
  BoundingBoxPtr getTowerCombinedBoundingBox(uint towerID);

  //  retrieves a tower in the same network as tower of towerID
  //  findClosest => returns the closest
  //  qualifyingFunc => will skip towers that this function returns false on
  typedef std::function<bool(TowerPtr tower)> TowerQualifyingFunc;
  TowerPtr findConnectedTower(uint towerID, bool findClosest, TowerQualifyingFunc qualifyingFunc) const;

  TowerPtr findClosestConnectedPowerSrc(uint towerID, bool mustHaveEnergy = false) const;
  TowerPtr findClosestConnectedMiner(uint towerID, bool mustHaveEnergy = false) const;
  void transferEnergy(Tower* srcTower, Tower* targetTower, uint amount) const;
  const std::set<uint>* combatTowerGetNetworksInRange(uint towerID) const;
  long timeBetweenEnergyTransfers() const { return 1000; };

  Vector3D terrainHitTest(uint cursorX, uint cursorY, bool* isLand = nullptr) const;
  bool isPositionLand(const Vector3D& position) const;

  void displayTowerRangeField(Tower* tower);
  void hideTowerRangeField(uint towerID);
  void displayAllRangeFields();
  void hideAllRangeFields();

  TowerPtr getFocusedTower();
  void doTowerDamageEffect(const Tower* tower, const Vector3D& effectColour = Vector3D(0.9, 0, 0));

  void getGameState(TOGameState* state);

  inline static TOGameContext* cast(GameContext* context)
    {
    TOGameContext* toContext = dynamic_cast<TOGameContext*>(context);
    ASSERT(toContext, "Given game context wasn't a TOGameContext!");
    return toContext;
    }

protected:
  void initSurface();
  void rebuildCombatTowerNetworksMap();
  };
