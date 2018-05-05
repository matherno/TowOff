#pragma once

class TOGameContext;

#include <GameSystem/GameSystem.h>
#include <GameSystem/GameContextImpl.h>
#include <ParticleSystem/ParticleSystem.h>
#include <RenderSystem/RenderableTerrain.h>
#include <GameSystem/Timer.h>
#include "Tower.h"
#include "Projectile.h"
#include "TOInputHandler.h"
#include "HUDHandler.h"
#include "ConnectionManager.h"
#include "RangeFieldManager.h"
#include "SpecialEffectsHandler.h"
#include "TOGameSaveLoad.h"
#include "PauseMenuHandler.h"
#include "TowerSelectionManager.h"
#include "FogOfWarRenderable.h"
#include "FogOfWarHandler.h"
#include "TOSettings.h"
#include "Bot.h"
#include "BotPortal.h"

#define DRAW_STAGE_FOGOFWAR   (DRAW_STAGE_TRANSPARENT + 1)

/*
*   Sub-class of Game Context to capture the central state of the TowOff game
*/

typedef mathernogl::MappedList<std::pair<Vector3D, float>> VisibilityMarkersList;

class TOGameContext : public GameContextImpl
  {
private:
  std::shared_ptr<TOGameState> loadedGameState;
  TowerList towers;
  BotList botList;
  BotPortalList botPortalList;
  std::shared_ptr<RenderableTerrain> surfaceMesh;
  std::shared_ptr<RenderableTerrain> waterMesh;
  HUDHandler hudHandler;
  SpecialEffectsHandler specialEffectsHandler;
  std::shared_ptr<TOSettings> settingsHandler;
  std::shared_ptr<ConnectionManager> connectionManager;
  std::shared_ptr<RangeFieldManager> rangeFieldManager;
  std::shared_ptr<TowerSelectionManager> selectionManager;
  std::shared_ptr<TOInputHandler> toInputHandler;
  std::shared_ptr<PauseMenuHandler> pauseMenuHandler;
  std::shared_ptr<FogOfWarHandler> fogOfWarHandler;
  VisibilityMarkersList visibilityMarkers;
  uint nextVisibilityMarkerID = 1;

  //  maps combat towers to the networks (via relay towers) that they are within range of
  std::map<uint, std::set<uint>> combatTowerNetworks;

  //  maps tower ids to a list of their bounding box ids
  std::map<uint, std::list<uint>> towerBoundingBoxes;

  //  maps tower ids to their displayed range field
  std::map<uint, uint> towerRangeFields;

public:
  TOGameContext(const RenderContextPtr& renderContext, std::shared_ptr<TOSettings> settings, std::shared_ptr<TOGameState> loadedGameState = nullptr)
    : GameContextImpl(renderContext), settingsHandler(settings), loadedGameState(loadedGameState) {}
  virtual bool initialise() override;
  virtual void cleanUp() override;
  virtual void processInputStage() override;
  virtual void processUpdateStage() override;
  virtual void processDrawStage() override;

  virtual FontPtr getDefaultFont() override;

  Vector3D getPlayerColour(uint num) const;
  HUDHandler* getHUDHandler(){ return &hudHandler; }
  TowerSelectionManager* getSelectionManager(){ return selectionManager.get(); }
  TOSettings* getSettings(){ return settingsHandler.get(); }
  void displayPauseMenu();
  Vector3D getCameraFocalPosition() const;
  void setFogOfWarState(bool isOn);

  TowerPtr getTower(uint id);
  const TowerList* getTowers() const { return &towers; }
  void forEachTower(std::function<void(TowerPtr tower)> func);
  TowerPtr findClosestTowerTo(const Tower* tower);
  TowerPtr findClosestTower(const Vector3D& position);
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
  bool isPositionOnMap(const Vector3D& pos) const;
  float getMapWidth() const;
  uint addVisibilityMarker(const Vector3D& position, float radius);
  void removeVisibilityMarker(uint id);
  const VisibilityMarkersList* getVisibilityMarkers() const { return &visibilityMarkers; }

  Vector3D terrainHitTest(uint cursorX, uint cursorY, bool* isLand = nullptr) const;
  bool isPositionLand(const Vector3D& position) const;

  void displayTowerRangeField(Tower* tower);
  void hideTowerRangeField(uint towerID);
  void displayAllRelayRanges();
  void hideAllRangeFields();

  TowerPtr getFocusedTower();
  void doTowerDamageEffect(const Tower* tower);
  void getGameState(TOGameState* state);

  BotPtr createBot(uint botType, const Vector3D& position);
  BotPtr getBot(uint id);
  void removeBot(uint id);
  const BotList* getBotList() const { return &botList; }
  BotPtr findClosestBot(const Vector3D& position, float range = -1);    // range of < 0 is infinite
  void doBotDamageEffect(const Bot* bot);
  BotPortalPtr createBotPortal(const Vector3D& position);
  void removeBotPortal(uint id);
  const BotPortalList* getBotPortalList() const { return &botPortalList; }
  BotPortalPtr findClosestBotPortal(const Vector3D& position, float range = -1);    // range of < 0 is infinite
  TowerTargetPtr findClosestTowerTarget(const Vector3D& position, float range = -1);    // range of < 0 is infinite

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
