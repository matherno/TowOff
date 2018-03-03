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

#define LAND_HEIGHT 0
#define WATER_HEIGHT -1
#define WATER_FLOOR_HEIGHT -5

bool TOGameContext::initialise()
  {
  renderConfig.windowName = "TowOff";
  bool success = GameContextImpl::initialise();
  InputHandlerPtr inputHandler(new TOInputHandler(getInputManager()->getNextHandlerID(), Vector3D(0, 70, 60), -20, 0, -45));
  addInputHandler(inputHandler);
  initSurface(7);
  hudHandler.initialiseUI(this);
  specialEffectsHandler.initialise(this);
  connectionManager.reset(new ConnectionManager(getNextActorID()));
  addActor(connectionManager);
  rangeFieldManager.reset(new RangeFieldManager(getNextActorID()));
  addActor(rangeFieldManager);
  return success;
  }

void TOGameContext::cleanUp()
  {
  if (surfaceMesh)
    {
    getRenderContext()->getRenderableSet()->removeRenderable(surfaceMesh->getID());
    surfaceMesh->cleanUp(getRenderContext());
    specialEffectsHandler.cleanUp(this);
    removeActor(connectionManager->getID());
    removeActor(rangeFieldManager->getID());
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

Player* TOGameContext::getPlayer(uint num)
  {
  ASSERT(num > 0 && num <= players.size(), "Not a valid player number: " + std::to_string(num));
  return players[num-1].get();
  }

Player* TOGameContext::createPlayer()
  {
  uint playerNum = (uint)players.size() - 1;
  players.push_back(std::make_shared<Player>(getNextActorID(), playerNum));
  addActor(players[playerNum]);
  return players[playerNum].get();
  }

TowerPtr TOGameContext::getTower(uint id)
  {
  if (towers.contains(id))
    return towers.get(id);
  return nullptr;
  }

TowerPtr TOGameContext::getClosestTowerTo(const Tower* tower, bool onlyEnemies)
  {
  TowerPtr closestTower;
  double closestTowerDist;
  const Vector3D srcTowerPos = tower->getPosition();
  for (TowerPtr& targetTower : *towers.getList())
    {
    double distance = srcTowerPos.distanceToPoint(targetTower->getPosition());
    if (!closestTower || distance < closestTowerDist)
      {
      if (targetTower->getID() != tower->getID())
        {
        if (!onlyEnemies || tower->getPlayerNum() != targetTower->getPlayerNum())
          {
          closestTower = targetTower;
          closestTowerDist = distance;
          }
        }
      }
    }
  return closestTower;
  }

void TOGameContext::removeTower(uint id)
  {
  if (focusedTower && focusedTower->getID() == id)
    unfocusTower();

  TowerPtr tower = getTower(id);
  towers.remove(id);
  removeActor(id);
  connectionManager->removeTower(id);
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

  std::list<BoundingBoxPtr> boundingBoxes;
  TowerFactory::createTowerBoundingBoxes(towerType, position, &boundingBoxes);
  for (BoundingBoxPtr box : boundingBoxes)
    {
    uint boxID = getBoundingBoxManager()->addBoundingBox(box, tower->getID());
    towerBoundingBoxes[tower->getID()].push_back(boxID);
    }
  return tower;
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

void TOGameContext::initSurface(uint size)
  {
  RenderContext* renderContext = getRenderContext();
  std::shared_ptr<HeightMap> heightMap(new HeightMap());
  HeightMapFactory::createDiamondSquareMap(heightMap.get(), size, 20, 0.75);

  for (float& height : heightMap->heights)
    {
    if (height > 0)
      height = LAND_HEIGHT;
    else
      height = WATER_FLOOR_HEIGHT;
    }

  const float cellSize = 1;
  const uint numCells = (uint)pow(2, (int)size);
  const float translation = (float)numCells*cellSize*-0.5f;
  surfaceMesh.reset(new RenderableTerrain(renderContext->getNextRenderableID(), heightMap, cellSize));
  surfaceMesh->setMultiColour(Vector3D(0.2, 0.4, 0.2), Vector3D(0.1, 0.3, 0.0));
  surfaceMesh->getTransform()->translate(Vector3D(translation, 0, translation));
  surfaceMesh->setClippingPlane(Vector4D(0, 1, 0, -WATER_HEIGHT));
  surfaceMesh->initialise(renderContext);
  renderContext->getRenderableSet()->addRenderable(surfaceMesh);

  std::shared_ptr<RenderableTerrain> waterMesh;
  waterMesh.reset(new RenderableTerrain(renderContext->getNextRenderableID(), numCells, cellSize));
  waterMesh->setSingleColour(Vector3D(0.2, 0.2, 0.5));
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

void TOGameContext::displayTowerRangeField(Tower* tower)
  {
  float radius = TowerFactory::getTowerRange(tower->getTowerType());
  if (radius > 0)
    {
    hideTowerRangeField(tower);
    towerRangeFields[tower->getID()] = rangeFieldManager->createRangeField(tower->getPosition() + Vector3D(0, 0.02, 0), radius, Vector3D(0.1, 0.5, 0.5));
    }
  }

void TOGameContext::hideTowerRangeField(Tower* tower)
  {
  if (towerRangeFields.count(tower->getID()) > 0)
    {
    rangeFieldManager->removeRangeField(towerRangeFields[tower->getID()]);
    towerRangeFields.erase(tower->getID());
    }
  }

void TOGameContext::displayAllRangeFields()
  {
  for (TowerPtr tower : *towers.getList())
    {
    if (!tower->isUnderConstruction())
      displayTowerRangeField(tower.get());
    }
  }

void TOGameContext::hideAllRangeFields()
  {
  for (auto pair : towerRangeFields)
    rangeFieldManager->removeRangeField(pair.second);
  towerRangeFields.clear();
  }

void TOGameContext::focusTower(TowerPtr tower)
  {
  unfocusTower();
  displayTowerRangeField(tower.get());
  focusedTower = tower;
  }

void TOGameContext::focusTower(uint towerID)
  {
  if (TowerPtr tower = getTower(towerID))
    focusTower(tower);
  }

void TOGameContext::unfocusTower()
  {
  if (focusedTower)
    {
    hideTowerRangeField(focusedTower.get());
    focusedTower = nullptr;
    }
  }

FontPtr TOGameContext::getDefaultFont()
  {
  return getRenderContext()->createFont(FONT_DEFAULT_FNT, FONT_DEFAULT_GLYPHS);
  }

void TOGameContext::doTowerDamageEffect(const Tower* tower, const Vector3D& effectColour)
  {
  specialEffectsHandler.towerDamageEffect(this, tower, effectColour);
  }

