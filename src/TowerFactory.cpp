//
// Created by matt on 28/01/18.
//

#include "TowerFactory.h"
#include "Tower.h"
#include "ProjectileWeapon.h"
#include "Resources.h"
#include "InstantWeapon.h"

const std::vector<TowerType> towerTypes =
  {
    TowerType{"Home Base",       IMAGE_ICON_HOME_BASE,    TowerFactory::createHomeBase},
    TowerType{"Basic Tower A",   IMAGE_ICON_BASIC_TOWER,  TowerFactory::createBasicTower},
    TowerType{"Pylon",           IMAGE_ICON_PYLON,        TowerFactory::createPylon},
    TowerType{"Miner",           IMAGE_ICON_MINER,        TowerFactory::createMiner},
    TowerType{"Enemy Tower",     IMAGE_ICON_ENEMY_TOWER,  TowerFactory::createBasicTower},
  };


const std::vector<TowerType>* TowerFactory::getTowerTypeList()
  {
  return &towerTypes;
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
  ASSERT(towerType < towerTypes.size(), "Invalid tower type number");
  return towerTypes[towerType].createFunction(id, position);
  }

TowerPtr TowerFactory::createBasicTowerProj(uint id, const Vector3D& position)
  {
  std::unique_ptr<ProjectileWeapon> weapon(new ProjectileWeapon());
  weapon->setCooldownTime(1000);
  weapon->setShootForce(100);
  weapon->setGravityForce(0);
  weapon->setCreateProjectileFunc([](uint id)->ProjectilePtr
                                    {
                                    return createFootballProjectile(id);
                                    });

  TowerPtr tower(new Tower(id, Tower::combat, MESH_BASIC_TOWER_BASE, MESH_BASIC_TOWER_TURRET));
  tower->setPosition(position);
  tower->setConnectOffset(Vector3D(0, 1, 0));
  tower->setTargetOffset(Vector3D(0, 1, 0));
  tower->setShootOffset(Vector3D(0, 2, -1));
  tower->setWeapon(std::move(weapon));
  return tower;
  }

TowerPtr TowerFactory::createBasicTower(uint id, const Vector3D& position)
  {
  std::unique_ptr<InstantWeapon> weapon(new InstantWeapon());
  weapon->setCooldownTime(1000);
  weapon->setBeamRadius(0.2);

  TowerPtr tower(new Tower(id, Tower::combat, MESH_BASIC_TOWER_BASE, MESH_BASIC_TOWER_TURRET));
  tower->setPosition(position);
  tower->setConnectOffset(Vector3D(0, 1, 0));
  tower->setTargetOffset(Vector3D(0, 0.5, 0));
  tower->setShootOffset(Vector3D(0, 1.45, -1));
  tower->setWeapon(std::move(weapon));
  return tower;
  }

TowerPtr TowerFactory::createHomeBase(uint id, const Vector3D& position)
  {
  TowerPtr tower(new Tower(id, Tower::storage, MESH_HOME_BASE));
  tower->setConnectOffset(Vector3D(-0.61, 5.12, 0.61));
  tower->setTargetOffset(Vector3D(0, 0.5, 0));
  tower->setPosition(position);
  return tower;
  }

TowerPtr TowerFactory::createPylon(uint id, const Vector3D& position)
  {
  TowerPtr tower(new Tower(id, Tower::relay, MESH_PYLON));
  tower->setConnectOffset(Vector3D(0, 4.02, 0));
  tower->setTargetOffset(Vector3D(0, 0.5, 0));
  tower->setPosition(position);
  tower->setConnectRadius(20);
  return tower;
  }

TowerPtr TowerFactory::createMiner(uint id, const Vector3D& position)
  {
  TowerPtr tower(new Tower(id, Tower::miner, MESH_MINER_BASE, MESH_MINER_TURRET));
  tower->setConnectOffset(Vector3D(0, 2.61, 0));
  tower->setTargetOffset(Vector3D(0, 0.5, 0));
  tower->setPosition(position);
  return tower;
  }
