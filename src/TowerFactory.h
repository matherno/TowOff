#pragma once

#include <src/RenderSystem/RenderSystem.h>
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
  };

class TowerFactory
  {
public:
  static const std::map<uint, TowerType>* getTowerTypeMap();
  static const TowerType* getTowerType(uint towerType);
  static float getRelayPowerRange(uint towerType);
  static Tower::TowerFunction getTowerFunction(uint towerType);

  static TowerPtr createTower(uint towerType, uint id, const Vector3D& position);
  static TowerPtr createBasicTowerProj(uint id, uint towerType, const Vector3D& position);
  static TowerPtr createBasicTower(uint id, uint towerType, const Vector3D& position);
  static TowerPtr createHomeBase(uint id, uint towerType, const Vector3D& position);
  static TowerPtr createPylon(uint id, uint towerType, const Vector3D& position);
  static TowerPtr createMiner(uint id, uint towerType, const Vector3D& position);

  static void createTowerBoundingBoxes(uint towerType, const Vector3D& position, std::list<BoundingBoxPtr>* boundingBoxes);

  };
