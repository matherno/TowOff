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
    TowerType{"Basic Tower A", IMAGE_ICON_BASIC_TOWER, TowerFactory::createBasicTowerA},
    TowerType{"Basic Tower B", IMAGE_ICON_BASIC_TOWER_DUP, TowerFactory::createBasicTowerB},
    TowerType{"Basic Tower B", IMAGE_ICON_BASIC_TOWER_DUP, TowerFactory::createBasicTowerB},
    TowerType{"Basic Tower B", IMAGE_ICON_BASIC_TOWER_DUP, TowerFactory::createBasicTowerB},
    TowerType{"Basic Tower B", IMAGE_ICON_BASIC_TOWER_DUP, TowerFactory::createBasicTowerB},
    TowerType{"Basic Tower B", IMAGE_ICON_BASIC_TOWER_DUP, TowerFactory::createBasicTowerB},
    TowerType{"Basic Tower B", IMAGE_ICON_BASIC_TOWER_DUP, TowerFactory::createBasicTowerB},
  };


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

  TowerPtr tower(new Tower(id, MESH_BASIC_TOWER_BASE, MESH_BASIC_TOWER_TURRET));
  tower->setPosition(position);
  tower->setTargetOffset(Vector3D(0, 1, 0));
  tower->setShootOffset(Vector3D(0, 2, -1));
  tower->setWeapon(std::move(weapon));
  return tower;
  }

TowerPtr TowerFactory::createBasicTowerA(uint id, const Vector3D& position)
  {
  std::unique_ptr<InstantWeapon> weapon(new InstantWeapon());
  weapon->setCooldownTime(1000);

  TowerPtr tower(new Tower(id, MESH_BASIC_TOWER_BASE, MESH_BASIC_TOWER_TURRET));
  tower->setPosition(position);
  tower->setTargetOffset(Vector3D(0, 1, 0));
  tower->setShootOffset(Vector3D(0, 2, -2));
  tower->setWeapon(std::move(weapon));
  tower->setPlayerNum(1);
  return tower;
  }

TowerPtr TowerFactory::createBasicTowerB(uint id, const Vector3D& position)
  {
  std::unique_ptr<InstantWeapon> weapon(new InstantWeapon());
  weapon->setCooldownTime(1000);

  TowerPtr tower(new Tower(id, MESH_BASIC_TOWER_BASE, MESH_BASIC_TOWER_TURRET));
  tower->setPosition(position);
  tower->setTargetOffset(Vector3D(0, 1, 0));
  tower->setShootOffset(Vector3D(0, 2, -2));
  tower->setWeapon(std::move(weapon));
  tower->setPlayerNum(2);
  return tower;
  }

const std::vector<TowerType>* TowerFactory::getTowerTypeList()
  {
  return &towerTypes;
  }
