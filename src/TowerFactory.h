#pragma once

#include <RenderSystem/RenderSystem.h>
#include "Tower.h"

/*
*   
*/

struct TowerType
  {
  string name;
  string iconFilePath;
  string baseMeshFilePath;
  string turretMeshFilePath;
  Vector3D connectOffset;
  float connectRadius;
  float hitRadius;
  };

class TowerFactory
  {
public:
  static const std::map<uint, TowerType>* getTowerTypeMap();
  static const TowerType* getTowerType(uint towerType);
  static float getTowerRange(uint towerType);
  static float getRelayPowerRange(uint towerType);
  static float getCombatRange(uint towerType);
  static float getCombatMinRange(uint towerType);
  static Tower::TowerFunction getTowerFunction(uint towerType);
  static bool canConstructTowerType(uint towerType);
  static uint getStartTowerTypeID();

  static TowerPtr createTower(uint towerType, uint id, const Vector3D& position);
  static TowerPtr createUnderConstructTower(uint towerType, uint id, const Vector3D& position);
  static TowerPtr createBasicTower(uint id, uint towerType, const Vector3D& position);
  static TowerPtr createHomeBase(uint id, uint towerType, const Vector3D& position);
  static TowerPtr createPylon(uint id, uint towerType, const Vector3D& position);
  static TowerPtr createMiner(uint id, uint towerType, const Vector3D& position);
  static TowerPtr createMachineGunTower(uint id, uint towerType, const Vector3D& position);
  static TowerPtr createMortar(uint id, uint towerType, const Vector3D& position);

  static void createTowerBoundingBoxes(uint towerType, const Vector3D& position, std::list<BoundingBoxPtr>* boundingBoxes);

  };
