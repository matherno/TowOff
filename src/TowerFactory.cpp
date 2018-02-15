//
// Created by matt on 28/01/18.
//

#include "TowerFactory.h"
#include "Tower.h"
#include "ProjectileWeapon.h"
#include "Resources.h"
#include "InstantWeapon.h"
#include "TowerFunctionalities.h"

#define TOWER_HOMEBASE  0
#define TOWER_BASIC     1
#define TOWER_PYLON     2
#define TOWER_MINER     3
#define TOWER_ENEMY     4

const std::map<uint, TowerType> towerTypes =
  {
  {TOWER_HOMEBASE, TowerType{"Home Base",       IMAGE_ICON_HOME_BASE,    TowerFactory::createHomeBase}},
  {TOWER_BASIC,    TowerType{"Basic Tower A",   IMAGE_ICON_BASIC_TOWER,  TowerFactory::createBasicTower}},
  {TOWER_PYLON,    TowerType{"Pylon",           IMAGE_ICON_PYLON,        TowerFactory::createPylon}},
  {TOWER_MINER,    TowerType{"Miner",           IMAGE_ICON_MINER,        TowerFactory::createMiner}},
  {TOWER_ENEMY,    TowerType{"Enemy Tower",     IMAGE_ICON_ENEMY_TOWER,  TowerFactory::createBasicTower}},
  };


const std::map<uint, TowerType>* TowerFactory::getTowerTypeMap()
  {
  return &towerTypes;
  }

const TowerType* TowerFactory::getTowerType(uint towerType)
  {
  ASSERT(towerTypes.count(towerType) > 0, "Invalid tower type number");
  return &towerTypes.at(towerType);
  }

ProjectilePtr createFootballProjectile(uint id)
  {
  ProjectilePtr projectile(new Projectile(id));
  projectile->setScale(2);
  projectile->setMeshFilePath(MESH_FOOTBALL_PROJECTILE);
  projectile->setDragEffect(0.5);
  projectile->setTimeToLive(1000);
  return projectile;
  }

TowerPtr TowerFactory::createTower(uint towerType, uint id, const Vector3D& position)
  {

  return getTowerType(towerType)->createFunction(id, towerType, position);
  }

TowerPtr TowerFactory::createBasicTowerProj(uint id, uint towerType, const Vector3D& position)
  {
  std::unique_ptr<ProjectileWeapon> weapon(new ProjectileWeapon());
  weapon->setCooldownTime(1000);
  weapon->setShootForce(100);
  weapon->setGravityForce(0);
  weapon->setCreateProjectileFunc([](uint id)->ProjectilePtr
                                    {
                                    return createFootballProjectile(id);
                                    });

  TowerFunctionalityCombat* function = new TowerFunctionalityCombat();
  function->setShootOffset(Vector3D(0, 2, -1));
  function->setWeapon(std::move(weapon));

  TowerPtr tower(new Tower(id, towerType, std::move(TowerFunctionalityPtr(function))));
  tower->setPosition(position);
  tower->setConnectOffset(Vector3D(0, 1, 0));
  tower->setTargetOffset(Vector3D(0, 1, 0));
  tower->setBaseModelFilePath(MESH_BASIC_TOWER_BASE);
  tower->setTurretModelFilePath(MESH_BASIC_TOWER_TURRET);
  return tower;
  }

TowerPtr TowerFactory::createBasicTower(uint id, uint towerType, const Vector3D& position)
  {
  std::unique_ptr<InstantWeapon> weapon(new InstantWeapon());
  weapon->setCooldownTime(1000);
  weapon->setBeamRadius(0.2);
  if (towerType == TOWER_ENEMY)
    weapon->setEnergyPerShot(0);

  TowerFunctionalityCombat* function = new TowerFunctionalityCombat();
  function->setShootOffset(Vector3D(0, 1.45, -1));
  function->setWeapon(std::move(weapon));

  TowerPtr tower(new Tower(id, towerType, std::move(TowerFunctionalityPtr(function))));
  tower->setPosition(position);
  tower->setConnectOffset(Vector3D(0, 1, 0));
  tower->setTargetOffset(Vector3D(0, 0.5, 0));
  tower->setBaseModelFilePath(MESH_BASIC_TOWER_BASE);
  tower->setTurretModelFilePath(MESH_BASIC_TOWER_TURRET);
  return tower;
  }

TowerPtr TowerFactory::createHomeBase(uint id, uint towerType, const Vector3D& position)
  {
  TowerFunctionalityStorage* function = new TowerFunctionalityStorage();

  TowerPtr tower(new Tower(id, towerType, std::move(TowerFunctionalityPtr(function))));
  tower->setConnectOffset(Vector3D(-0.61, 5.12, 0.61));
  tower->setTargetOffset(Vector3D(0, 0.5, 0));
  tower->setPosition(position);
  tower->setBaseModelFilePath(MESH_HOME_BASE);
  return tower;
  }

TowerPtr TowerFactory::createPylon(uint id, uint towerType, const Vector3D& position)
  {
  TowerFunctionalityRelay* function = new TowerFunctionalityRelay();

  TowerPtr tower(new Tower(id, towerType, std::move(TowerFunctionalityPtr(function))));
  tower->setConnectOffset(Vector3D(0, 4.02, 0));
  tower->setTargetOffset(Vector3D(0, 0.5, 0));
  tower->setPosition(position);
  tower->setConnectRadius(20);
  tower->setBaseModelFilePath(MESH_PYLON);
  return tower;
  }

TowerPtr TowerFactory::createMiner(uint id, uint towerType, const Vector3D& position)
  {
  TowerFunctionalityMiner* function = new TowerFunctionalityMiner();
  function->setEnergyTransferRate(20);

  TowerPtr tower(new Tower(id, towerType, std::move(TowerFunctionalityPtr(function))));
  tower->setConnectOffset(Vector3D(0, 2.61, 0));
  tower->setTargetOffset(Vector3D(0, 0.5, 0));
  tower->setPosition(position);
  tower->setBaseModelFilePath(MESH_MINER_BASE);
  tower->setTurretModelFilePath(MESH_MINER_TURRET);
  return tower;
  }

float TowerFactory::getRelayPowerRange(uint towerType)
  {
  switch (towerType)
    {
    case TOWER_PYLON:
      return 20;
    }
  return 0;
  }

void TowerFactory::createTowerBoundingBoxes(uint towerType, const Vector3D& position, std::list<BoundingBoxPtr>* boundingBoxes)
  {
  auto AddBoundingBox = [position, boundingBoxes](const Vector3D& min, const Vector3D& max)
    {
    BoundingBox* bb = new BoundingBox(position + min, position + max);
    boundingBoxes->push_back(BoundingBoxPtr(bb));
    };

  switch (towerType)
    {
    case TOWER_BASIC:
    case TOWER_ENEMY:
      AddBoundingBox(Vector3D(-0.7, 0, -0.7), Vector3D(0.7, 1.3, 0.7));
      break;
    case TOWER_PYLON:
      AddBoundingBox(Vector3D(-0.8, 0, -0.8), Vector3D(0.8, 4.1, 0.8));
      break;
    case TOWER_MINER:
      AddBoundingBox(Vector3D(-1.4, 0, -1.4), Vector3D(1.4, 0.5, 1.4));
      AddBoundingBox(Vector3D(-0.8, 0, -0.45), Vector3D(0.8, 2.8, 0.45));
      break;
    case TOWER_HOMEBASE:
      AddBoundingBox(Vector3D(-2.6, 0, -2.6), Vector3D(2.6, 0.65, 2.6));
      AddBoundingBox(Vector3D(-1.3, 0, -1.3), Vector3D(1.3, 5.5, 1.3));
      break;
    }
  }

