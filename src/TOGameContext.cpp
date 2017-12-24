//
// Created by matt on 3/12/17.
//

#include <src/RenderSystem/RenderableMesh.h>
#include <src/RenderSystem/RenderableTerrain.h>
#include "TOGameContext.h"
#include "Resources.h"
#include "ProjectileWeapon.h"
#include "InstantWeapon.h"

bool TOGameContext::initialise()
  {
  bool success = GameContextImpl::initialise();
  initSurface(10, 15);
  initDamageParticleSystem();
  return success;
  }

void TOGameContext::cleanUp()
  {
  if (surfaceMesh)
    {
    getRenderContext()->getRenderableSet()->removeRenderable(surfaceMesh->getID());
    surfaceMesh->cleanUp(getRenderContext());
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
  towers.remove(id);
  removeActor(id);
  }

TowerPtr TOGameContext::createBasicTower(const Vector3D& position)
  {
  std::unique_ptr<InstantWeapon> weapon(new InstantWeapon());
  weapon->setCooldownTime(1000);

  TowerPtr tower(new Tower(getNextActorID(), MESH_BASIC_TOWER_BASE, MESH_BASIC_TOWER_TURRET));
  tower->setPosition(position);
  tower->setTargetOffset(Vector3D(0, 1, 0));
  tower->setShootOffset(Vector3D(0, 2, -2));
  tower->setWeapon(std::move(weapon));
  towers.add(tower, tower->getID());
  addActor(tower);
  return tower;
  }

TowerPtr TOGameContext::createBasicTowerProj(const Vector3D& position)
  {
  std::unique_ptr<ProjectileWeapon> weapon(new ProjectileWeapon());
  weapon->setCooldownTime(1000);
  weapon->setShootForce(100);
  weapon->setGravityForce(0);
  weapon->setCreateProjectileFunc([this](uint id)->ProjectilePtr
                                    {
                                    return createFootballProjectile(id);
                                    });

  TowerPtr tower(new Tower(getNextActorID(), MESH_BASIC_TOWER_BASE, MESH_BASIC_TOWER_TURRET));
  tower->setPosition(position);
  tower->setTargetOffset(Vector3D(0, 1, 0));
  tower->setShootOffset(Vector3D(0, 2, -1));
  tower->setWeapon(std::move(weapon));
  towers.add(tower, tower->getID());
  addActor(tower);
  return tower;
  }

ProjectilePtr TOGameContext::createFootballProjectile(uint id)
  {
  ProjectilePtr projectile(new Projectile(id));
  projectile->setScale(2);
  projectile->setMeshFilePath(MESH_FOOTBALL_PROJECTILE);
  projectile->setDragEffect(0.5);
  projectile->setTimeToLive(1000);
  return projectile;
  }

Vector3D TOGameContext::getPlayerColour(uint num)
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

void TOGameContext::initSurface(uint numCells, float cellSize)
  {
  RenderContext* renderContext = getRenderContext();
  RenderableTerrain* terrain = new RenderableTerrain(renderContext->getNextRenderableID(), numCells, cellSize);
  terrain->setMultiColour(Vector3D(0.2, 0.4, 0.2), Vector3D(0.1, 0.3, 0.0));
  surfaceMesh.reset(terrain);
  surfaceMesh->getTransform()->translate(Vector3D(numCells*cellSize*-0.5f, 0, numCells*cellSize*-0.5f));
  surfaceMesh->initialise(renderContext);
  renderContext->getRenderableSet()->addRenderable(surfaceMesh);
  }

void TOGameContext::initDamageParticleSystem()
  {
  ParticleSystem* system = new ParticleSystem(getNextActorID());
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
