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
  std::function<TowerPtr(uint, const Vector3D&)> createFunction;
  };

class TowerFactory
  {
public:
  static TowerPtr createTower(uint towerType, uint id, const Vector3D& position);
  static TowerPtr createBasicTowerProj(uint id, const Vector3D& position);
  static TowerPtr createBasicTower(uint id, const Vector3D& position);
  static TowerPtr createHomeBase(uint id, const Vector3D& position);
  static TowerPtr createPylon(uint id, const Vector3D& position);
  static TowerPtr createMiner(uint id, const Vector3D& position);

  static const std::vector<TowerType>* getTowerTypeList();
  };
