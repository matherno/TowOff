//
// Created by matherno on 3/09/17.
//

#pragma once

#include "mathernogl/maths/Maths.h"

namespace mathernogl {

void loadObj(std::string filePath, std::vector<int>* indices, std::vector<Vector3D>* verts, std::vector<Vector3D>* norms, std::vector<Vector2D>* texCoords, std::vector<Vector3D>* faceCols = nullptr, bool onlyFirstShape = true);
void createCube(float width, std::vector<Vector3D>* verts);
void createCube(float minX, float maxX, float minY, float maxY, float minZ, float maxZ, std::vector<Vector3D>* verts);
void createGrid(uint numCellsX, uint numCellsY, float cellSize, std::vector<Vector3D>* verts);
void createGridHeightMapped(uint numCellsX, uint numCellsY, float cellSize, std::vector<float>* heightMap, std::vector<Vector3D>* verts);

}

