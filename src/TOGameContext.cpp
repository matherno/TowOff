//
// Created by matt on 3/12/17.
//

#include <RenderSystem/RenderableMesh.h>

#include "TOGameContext.h"
#include "Resources.h"
#include "ProjectileWeapon.h"
#include "InstantWeapon.h"
#include "TOInputHandler.h"
#include "TowerFactory.h"
#include "UnderConstructTower.h"
#include "BotFactory.h"

#define LAND_HEIGHT 0
#define WATER_HEIGHT -1
#define WATER_FLOOR_HEIGHT -5

bool TOGameContext::initialise()
  {
  bool success = GameContextImpl::initialise();
  getRenderContext()->registerDrawStage(DRAW_STAGE_FOGOFWAR);

  if (loadedGameState)
    toInputHandler.reset(new TOInputHandler(getInputManager()->getNextHandlerID(), loadedGameState->cameraFocalPos, loadedGameState->cameraZoomFactor, loadedGameState->cameraRotation, -45));
  else
    toInputHandler.reset(new TOInputHandler(getInputManager()->getNextHandlerID(), Vector3D(0, 0, 10), 80, 0, -45));
  addInputHandler(toInputHandler);

  initSurface();
  hudHandler.initialiseUI(this);
  specialEffectsHandler.initialise(this);
  connectionManager.reset(new ConnectionManager(getNextActorID()));
  addActor(connectionManager);
  rangeFieldManager.reset(new RangeFieldManager(getNextActorID()));
  addActor(rangeFieldManager);
  pauseMenuHandler.reset(new PauseMenuHandler(getNextActorID()));
  addActor(pauseMenuHandler);
  selectionManager.reset(new TowerSelectionManager(getNextActorID()));
  addActor(selectionManager);
  fogOfWarHandler.reset(new FogOfWarHandler(getNextActorID(), 52));
  addActor(fogOfWarHandler);

  addInitialTowers();
  if (towers.count() == 0)    // just for now, so that we have some map visible when we start with nothing
    addVisibilityMarker(Vector3D(0, 0, 0), 30);
  return success;
  }

void TOGameContext::cleanUp()
  {
  if (surfaceMesh)
    {
    getRenderContext()->getRenderableSet()->removeRenderable(surfaceMesh->getID());
    getRenderContext()->getRenderableSet()->removeRenderable(waterMesh->getID());
    surfaceMesh->cleanUp(getRenderContext());
    waterMesh->cleanUp(getRenderContext());

    hudHandler.cleanUp(this);
    specialEffectsHandler.cleanUp(this);
    removeActor(connectionManager->getID());
    removeActor(rangeFieldManager->getID());
    removeActor(pauseMenuHandler->getID());
    removeActor(fogOfWarHandler->getID());
    }
  GameContextImpl::cleanUp();
  }

void TOGameContext::processInputStage()
  {
  GameContextImpl::processInputStage();
  }

void TOGameContext::processUpdateStage()
  {
  hudHandler.updateUI(this);
  specialEffectsHandler.update(this);
  GameContextImpl::processUpdateStage();
  }

void TOGameContext::processDrawStage()
  {
  GameContextImpl::processDrawStage();
  }

TowerPtr TOGameContext::getTower(uint id)
  {
  if (towers.contains(id))
    return towers.get(id);
  return nullptr;
  }

TowerPtr findClosestTowerBase(const Vector3D& position, uint excludeTowerID, TowerList& towers)
  {
  TowerPtr closestTower;
  double closestTowerDist;
  for (TowerPtr& targetTower : *towers.getList())
    {
    double distance = position.distanceToPoint(targetTower->getPosition());
    if (!closestTower || distance < closestTowerDist)
      {
      if (targetTower->getID() != excludeTowerID)
        {
        closestTower = targetTower;
        closestTowerDist = distance;
        }
      }
    }
  return closestTower;
  }

TowerPtr TOGameContext::findClosestTowerTo(const Tower* tower)
  {
  return findClosestTowerBase(tower->getPosition(), tower->getID(), towers);
  }

TowerPtr TOGameContext::findClosestTower(const Vector3D& position)
  {
  return findClosestTowerBase(position, 0, towers);
  }

void TOGameContext::removeTower(uint id)
  {
  selectionManager->deselectTower(this, id);

  TowerPtr tower = getTower(id);
  towers.remove(id);
  removeActor(id);
  connectionManager->removeTower(id);
  fogOfWarHandler->refreshFOW(this);
  if (towerBoundingBoxes.count(id) > 0)
    {
    for (uint boxID : towerBoundingBoxes[id])
      getBoundingBoxManager()->removeBoundingBox(boxID);
    towerBoundingBoxes.erase(id);
    }

  if (tower->getFunction() == Tower::combat || tower->getFunction() == Tower::relay)
    rebuildCombatTowerNetworksMap();
  }

TowerPtr TOGameContext::createTower(uint towerType, const Vector3D& position, bool underConstruction)
  {
  TowerPtr tower;
  if (underConstruction)
    tower = TowerFactory::createUnderConstructTower(towerType, getNextActorID(), position);
  else
    tower = TowerFactory::createTower(towerType, getNextActorID(), position);

  towers.add(tower, tower->getID());
  addActor(tower);

  if (tower->getFunction() != Tower::combat)
    connectionManager->addTower(tower);
  if (tower->getFunction() == Tower::combat || tower->getFunction() == Tower::relay)
    rebuildCombatTowerNetworksMap();
  fogOfWarHandler->refreshFOW(this);

  std::list<BoundingBoxPtr> boundingBoxes;
  TowerFactory::createTowerBoundingBoxes(towerType, position, &boundingBoxes);
  for (BoundingBoxPtr box : boundingBoxes)
    {
    uint boxID = getBoundingBoxManager()->addBoundingBox(box, tower->getID());
    towerBoundingBoxes[tower->getID()].push_back(boxID);
    }
  return tower;
  }

void TOGameContext::forEachTower(std::function<void(TowerPtr tower)> func)
  {
  for (TowerPtr tower : *towers.getList())
    func(tower);
  }

void TOGameContext::addInitialTowers()
  {
  if (loadedGameState)
    {
    TowerPtr tower;
    for (TowerState& state : loadedGameState->towers)
      {
      tower = createTower(state.type, state.position, state.underConstruction);
      tower->setTowerState(&state);
      }
    }
  }

Vector3D TOGameContext::getPlayerColour(uint num) const
  {
  switch (num)
    {
    case 1:
      return Vector3D(0, 0, 0.5);
    case 2:
      return Vector3D(0, 0.5, 0);
    default:
      return Vector3D(0.5, 0.5, 0.5);
    }
  }

void TOGameContext::initSurface()
  {
  RenderContext* renderContext = getRenderContext();
  std::shared_ptr<HeightMap> heightMap;
  uint size = 7;

  if (loadedGameState)
    {
    heightMap = loadedGameState->terrainHeightMap;
    }
  else
    {
    heightMap.reset(new HeightMap());
    HeightMapFactory::createDiamondSquareMap(heightMap.get(), size, 20, 0.75);
    for (float& height : heightMap->heights)
      {
      if (height > 0)
        height = LAND_HEIGHT;
      else
        height = WATER_FLOOR_HEIGHT;
      }
    }

  const float cellSize = 1;
  const uint numCells = heightMap->width - 1;
  const float translation = (float)numCells*cellSize*-0.5f;
  surfaceMesh.reset(new RenderableTerrain(renderContext->getNextRenderableID(), heightMap, cellSize));
  surfaceMesh->setMultiColour(Vector3D(pow(0.2, 2.2), pow(0.4, 2.2), pow(0.2, 2.2)), Vector3D(pow(0.1, 2.2), pow(0.3, 2.2), pow(0.0, 2.2)));
  surfaceMesh->getTransform()->translate(Vector3D(translation, 0, translation));
  surfaceMesh->setClippingPlane(Vector4D(0, 1, 0, -WATER_HEIGHT));
  surfaceMesh->initialise(renderContext);
  renderContext->getRenderableSet()->addRenderable(surfaceMesh);

  waterMesh.reset(new RenderableTerrain(renderContext->getNextRenderableID(), numCells, cellSize));
  waterMesh->setSingleColour(Vector3D(pow(0.2, 2.2), pow(0.2, 2.2), pow(0.5, 2.2)));
  waterMesh->getTransform()->translate(Vector3D(translation, WATER_HEIGHT, translation));
  waterMesh->initialise(renderContext);
  renderContext->getRenderableSet()->addRenderable(waterMesh);
  }

Vector3D TOGameContext::terrainHitTest(uint cursorX, uint cursorY, bool* isLand /*= nullptr*/) const
  {
  ASSERT(LAND_HEIGHT == 0, "This assumes that land height is 0");
  Vector3D cursorWorldPos = getCursorWorldPos(cursorX, cursorY);
  Vector3D cursorViewDir = getViewDirectionAtCursor(cursorX, cursorY);
  ASSERT (cursorViewDir.y != 0, "View is parallel to terrain, can't hit!");
  double tValue = -1.0 * cursorWorldPos.y / cursorViewDir.y;
  Vector3D position = cursorWorldPos + (cursorViewDir * tValue);
  if (isLand)
    *isLand = isPositionLand(position);
  return position;
  }

bool TOGameContext::isPositionLand(const Vector3D& worldPos) const
  {
  float height = surfaceMesh->getHeightAt(worldPos);
  if (height != TERRAIN_OUT_OF_BOUNDS)
    return height >= LAND_HEIGHT;
  return false;
  }

bool isCombatTowerInRange(const TowerPtr& combatTower, const TowerPtr& relayTower)
  {
  float range = TowerFactory::getRelayPowerRange(relayTower->getTowerType());
  if (range <= 0)
    return false;
  return combatTower->getPosition().distanceToPoint(relayTower->getPosition()) <= range;
  }

void TOGameContext::rebuildCombatTowerNetworksMap()
  {
  combatTowerNetworks.clear();
  for (const TowerPtr& combatTower : *towers.getList())
    {

    if (combatTower->getFunction() != Tower::combat)
      continue;

    for (const TowerPtr& relayTower : *towers.getList())
      {
      if (relayTower->isUnderConstruction() || relayTower->getFunction() != Tower::relay)
        continue;

      if(isCombatTowerInRange(combatTower, relayTower))
        {
        const Network* network = connectionManager->getTowersNetwork(relayTower->getID());
        if (network)
          combatTowerNetworks[combatTower->getID()].insert(network->id);
        }
      }

    }
  }

TowerPtr TOGameContext::findConnectedTower(uint towerID, bool findClosest, TOGameContext::TowerQualifyingFunc qualifyingFunc) const
  {
  const TowerPtr tower = towers.get(towerID);
  if (!tower)
    return nullptr;

  bool isCombatTower = tower->getFunction() == Tower::combat;
  const std::set<uint>* combatTowerNetworks;
  if (isCombatTower)
    {
    combatTowerNetworks = combatTowerGetNetworksInRange(towerID);
    if (!combatTowerNetworks || combatTowerNetworks->empty())
      return nullptr;
    }

  TowerPtr resultTower = nullptr;
  double closestTowerDist;
  const Vector3D srcTowerPos = tower->getPosition();
  for (const TowerPtr& targetTower : *towers.getList())
    {
    bool connected = false;
    if (isCombatTower)
      {
      const Network* network = connectionManager->getTowersNetwork(targetTower->getID());
      if (network)
        connected = combatTowerNetworks->count(network->id) > 0;
      }
    else
      connected = connectionManager->areTowersConnected(towerID, targetTower->getID());

    if (!connected)
      continue;

    double distance = srcTowerPos.distanceToPoint(targetTower->getPosition());
    if (!findClosest || !resultTower || distance < closestTowerDist)
      {
      if (targetTower->getID() != towerID && qualifyingFunc(targetTower))
        {
        resultTower = targetTower;
        closestTowerDist = distance;
        if (!findClosest)
          return resultTower;
        }
      }
    }

  return resultTower;
  }

TowerPtr TOGameContext::findClosestConnectedPowerSrc(uint towerID, bool mustHaveEnergy) const
  {
  return findConnectedTower(towerID, true, [mustHaveEnergy, this](TowerPtr tower)
    {
    if (tower->isUnderConstruction())
      return false;
    if (mustHaveEnergy && !tower->hasEnergy())
      return false;
    return tower->isPowerSrc();
    });
  }

TowerPtr TOGameContext::findClosestConnectedMiner(uint towerID, bool mustHaveEnergy) const
  {
  return findConnectedTower(towerID, true, [mustHaveEnergy, this](TowerPtr tower)
    {
    if (tower->isUnderConstruction())
      return false;
    if (mustHaveEnergy && !tower->hasEnergy())
      return false;
    return tower->getFunction() == Tower::miner;
    });
  }

void TOGameContext::transferEnergy(Tower* srcTower, Tower* targetTower, uint amount) const
  {
  uint energyTaken = srcTower->takeEnergy(amount, false);
  uint energyLeftOver = energyTaken - targetTower->storeEnergy(energyTaken);
  if (energyLeftOver > 0)
    srcTower->storeEnergy(energyLeftOver);
  }

const std::set<uint>* TOGameContext::combatTowerGetNetworksInRange(uint towerID) const
  {
  const TowerPtr tower = towers.get(towerID);
  if (tower)
    {
    if (tower->getFunction() == Tower::combat && combatTowerNetworks.count(towerID) > 0)
      return &combatTowerNetworks.at(towerID);
    }
  return nullptr;
  }

bool TOGameContext::isPositionOnMap(const Vector3D& pos) const
  {
  return surfaceMesh->getHeightAt(pos) != TERRAIN_OUT_OF_BOUNDS;
  }

float TOGameContext::getMapWidth() const
  {
  return surfaceMesh->getCellSize() * (surfaceMesh->getHeightMap()->width - 1);
  }

void TOGameContext::displayTowerRangeField(Tower* tower)
  {
  float radius = TowerFactory::getTowerRange(tower->getTowerType());
  if (radius > 0)
    {
    Vector3D colour = Vector3D(0.1, 0.5, 0.5);
    if (tower->getFunction() == Tower::combat)
      colour.set(0.6, 0.2, 0.1);
    hideTowerRangeField(tower->getID());
    towerRangeFields[tower->getID()] = rangeFieldManager->createRangeField(tower->getPosition() + Vector3D(0, 0.02, 0), radius, colour);
    }
  }

void TOGameContext::hideTowerRangeField(uint towerID)
  {
  if (towerRangeFields.count(towerID) > 0)
    {
    rangeFieldManager->removeRangeField(towerRangeFields[towerID]);
    towerRangeFields.erase(towerID);
    }
  }

void TOGameContext::displayAllRelayRanges()
  {
  for (TowerPtr tower : *towers.getList())
    {
    if (!tower->isUnderConstruction() && tower->getFunction() == Tower::relay)
      displayTowerRangeField(tower.get());
    }
  }

void TOGameContext::hideAllRangeFields()
  {
  for (auto pair : towerRangeFields)
    rangeFieldManager->removeRangeField(pair.second);
  towerRangeFields.clear();
  }

FontPtr TOGameContext::getDefaultFont()
  {
  return getRenderContext()->getSharedFont(FONT_DEFAULT_FNT, FONT_DEFAULT_GLYPHS, FONT_DEFAULT_SCALING);
  }

void TOGameContext::doTowerDamageEffect(const Tower* tower)
  {
  specialEffectsHandler.towerDamageEffect(this, tower);
  }

void TOGameContext::getGameState(TOGameState* state)
  {
  state->cameraFocalPos = toInputHandler->getFocalPosition();
  state->cameraRotation = toInputHandler->getRotation();
  state->cameraZoomFactor = toInputHandler->getZoomOffset();
  state->terrainHeightMap = surfaceMesh->getHeightMap();
  for (TowerPtr tower : *towers.getList())
    {
    TowerState towerState;
    tower->getTowerState(&towerState);
    state->towers.push_back(towerState);
    }
  }

void TOGameContext::displayPauseMenu()
  {
  pauseMenuHandler->displayMenu(this);
  }

TowerPtr TOGameContext::getFocusedTower()
  {
  return selectionManager->getFirstSelectedTower();
  }

/*
 *  creates a new bounding box as a combination of all the towers bounding boxes, and returns it
 */
BoundingBoxPtr TOGameContext::getTowerCombinedBoundingBox(uint towerID)
  {
  if(towerBoundingBoxes.count(towerID) > 0 && towerBoundingBoxes[towerID].size() > 0)
    {
    BoundingBoxPtr combinedBox(new BoundingBox());
    for (uint boxID : towerBoundingBoxes[towerID])
      {
      if (BoundingBoxPtr box = getBoundingBoxManager()->getBoundingBox(boxID))
        combinedBox->addBoundingBox(box);
      }
    return combinedBox;
    }
  return nullptr;
  }

Vector3D TOGameContext::getCameraFocalPosition() const
  {
  return toInputHandler->getFocalPosition();
  }

uint TOGameContext::addVisibilityMarker(const Vector3D& position, float radius)
  {
  uint id = nextVisibilityMarkerID++;
  visibilityMarkers.add(std::make_pair(position, radius), id);
  if (fogOfWarHandler)
    fogOfWarHandler->refreshFOW(this);
  return id;
  }

void TOGameContext::removeVisibilityMarker(uint id)
  {
  if (visibilityMarkers.contains(id))
    visibilityMarkers.remove(id);
  }

BotPtr TOGameContext::createBot(uint botType, const Vector3D& position)
  {
  BotPtr bot = BotFactory::createBot(botType, getNextActorID(), position);
  if (!bot)
    return nullptr;

  botList.add(bot, bot->getID());
  addActor(bot);
  return bot;
  }

BotPtr TOGameContext::getBot(uint id)
  {
  if (botList.contains(id))
    return botList.get(id);
  return nullptr;
  }

void TOGameContext::removeBot(uint id)
  {
  if (botList.contains(id))
    {
    botList.remove(id);
    removeActor(id);
    }
  }

BotPtr TOGameContext::findClosestBot(const Vector3D& position, float range)
  {
  BotPtr closestBot;
  double closestBotDistance = range;
  bool infiniteRange = range < 0;
  for (BotPtr& targetBot : *botList.getList())
    {
    double distance = position.distanceToPoint(targetBot->getPosition());
    if ((infiniteRange && !closestBot) || distance < closestBotDistance)
      {
      closestBot = targetBot;
      closestBotDistance = distance;
      }
    }
  return closestBot;
  }

void TOGameContext::doBotDamageEffect(const Bot* bot)
  {
  specialEffectsHandler.botDamageEffect(this, bot);
  }

void TOGameContext::setFogOfWarState(bool isOn)
  {
  if (fogOfWarHandler)
    fogOfWarHandler->setState(isOn);
  }

