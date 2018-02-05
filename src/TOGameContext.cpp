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

#define LAND_HEIGHT 0
#define WATER_HEIGHT -1
#define WATER_FLOOR_HEIGHT -5

bool TOGameContext::initialise()
  {
  renderConfig.windowName = "TowOff";
  bool success = GameContextImpl::initialise();
  inputHandler.reset(new TOInputHandler(getInputManager()->getNextHandlerID(), Vector3D(0, 70, 60), -20, 0, -45));
  addInputHandler(inputHandler);
  initSurface(6);
  initDamageParticleSystem();
  initUI();
  connectionManager.reset(new ConnectionManager(getNextActorID()));
  addActor(connectionManager);
  return success;
  }

void TOGameContext::cleanUp()
  {
  if (surfaceMesh)
    {
    getRenderContext()->getRenderableSet()->removeRenderable(surfaceMesh->getID());
    surfaceMesh->cleanUp(getRenderContext());
    removeActor(connectionManager->getID());
    }
  GameContextImpl::cleanUp();
  }

void TOGameContext::processInputStage()
  {
  GameContextImpl::processInputStage();
  }

void TOGameContext::processUpdateStage()
  {
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
  TowerPtr tower = getTower(id);
  towers.remove(id);
  removeActor(id);
  connectionManager->removeTower(id);
  if (tower->getFunction() == Tower::combat || tower->getFunction() == Tower::relay)
    rebuildRelayInfluenceMap();
  }

TowerPtr TOGameContext::createTower(uint towerType, const Vector3D& position)
  {
  TowerPtr tower = TowerFactory::createTower(towerType, getNextActorID(), position);
  towers.add(tower, tower->getID());
  addActor(tower);
  if (tower->getFunction() != Tower::combat)
    connectionManager->addTower(tower);
  if (tower->getFunction() == Tower::combat || tower->getFunction() == Tower::relay)
    rebuildRelayInfluenceMap();
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
  HeightMapFactory::createDiamondSquareMap(heightMap.get(), size, 20, 0.7);

  for (float& height : heightMap->heights)
    {
    if (height > 0)
      height = LAND_HEIGHT;
    else
      height = WATER_FLOOR_HEIGHT;
    }

  const float cellSize = 2;
  const uint numCells = (uint)pow(2, (int)size);
  const float translation = (float)numCells*cellSize*-0.5f;
  surfaceMesh.reset(new RenderableTerrain(renderContext->getNextRenderableID(), heightMap, cellSize));
  surfaceMesh->setMultiColour(Vector3D(0.2, 0.4, 0.2), Vector3D(0.1, 0.3, 0.0));
  surfaceMesh->getTransform()->translate(Vector3D(translation, 0, translation));
  surfaceMesh->initialise(renderContext);
  renderContext->getRenderableSet()->addRenderable(surfaceMesh);

  std::shared_ptr<RenderableTerrain> waterMesh;
  waterMesh.reset(new RenderableTerrain(renderContext->getNextRenderableID(), numCells, cellSize));
  waterMesh->setSingleColour(Vector3D(0.2, 0.2, 0.5));
  waterMesh->getTransform()->translate(Vector3D(translation, WATER_HEIGHT, translation));
  waterMesh->initialise(renderContext);
  renderContext->getRenderableSet()->addRenderable(waterMesh);
  }

void TOGameContext::initDamageParticleSystem()
  {
  ParticleSystem* system = new ParticleSystem(getNextActorID(), true);
  system->setGravityAccel(0.00001);
  system->setTimeBetweenSpawns(7);
  system->setInitVelocity(0.007);
  system->setTimeAlive(300);
  system->setParticleSize(4);
  towerDamageParticles.reset(system);
  addActor(towerDamageParticles);
  }

void TOGameContext::doTowerDamageEffect(const Tower* tower)
  {
  towerDamageParticles->addEmitter(tower->getTargetPosition(), 50, Vector3D(0.9, 0, 0));
  }

Vector3D TOGameContext::terrainHitTest(uint cursorX, uint cursorY, bool* isLand /*= nullptr*/) const
  {
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

void TOGameContext::initUI()
  {
  hudHandler.initialiseUI(this);
  }

uint TOGameContext::getActivePlayer() const
  {
  return inputHandler->getActivePlayer();
  }

void TOGameContext::setActivePlayer(uint player)
  {
  inputHandler->setActivePlayer(player);
  }

bool TOGameContext::isConnectedToPowerSrc(uint towerID) const
  {
  const TowerPtr tower = towers.get(towerID);
  if (tower)
    {
    if (tower->getFunction() == Tower::combat)
      return relayInfluenceMap.count(towerID) > 0 && !relayInfluenceMap.at(towerID).empty();
    else
      return connectionManager->isTowerConnectedToPowerSrc(towerID);
    }
  return false;
  }

bool isCombatTowerInRange(const TowerPtr& combatTower, const TowerPtr& relayTower)
  {
  float range = TowerFactory::getRelayPowerRange(relayTower->getTowerType());
  if (range <= 0)
    return false;
  return combatTower->getPosition().distanceToPoint(relayTower->getPosition()) <= range;
  }

void TOGameContext::rebuildRelayInfluenceMap()
  {
  relayInfluenceMap.clear();
  for (const TowerPtr& combatTower : *towers.getList())
    {
    if (combatTower->getFunction() != Tower::combat)
      continue;

    for (const TowerPtr& relayTower : *towers.getList())
      {
      if (relayTower->getFunction() != Tower::relay)
        continue;

      if(isCombatTowerInRange(combatTower, relayTower))
        relayInfluenceMap[combatTower->getID()].push_back(relayTower->getID());
      }
    }
  }
