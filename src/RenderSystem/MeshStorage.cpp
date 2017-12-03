//
// Created by matherno on 19/11/17.
//

#include "MeshStorage.h"

using namespace mathernogl;

bool MeshStorage::initialiseVAO()
  {
  if (indices.empty() || vertices.empty() || normals.empty())
    return false;

  numVertices = 0;

  std::vector<float> bufferData;
  for (const int& index : indices)
    {
    Vector3D& vert = vertices[index];
    Vector3D& norm = normals[index];
    Vector2D tex(0);
    if (!texCoords.empty())
      tex = texCoords[index];
    bufferData.emplace_back(vert.x);
    bufferData.emplace_back(vert.y);
    bufferData.emplace_back(vert.z);
    bufferData.emplace_back(norm.x);
    bufferData.emplace_back(norm.y);
    bufferData.emplace_back(norm.z);
    bufferData.emplace_back(tex.x);
    bufferData.emplace_back(tex.y);
    ++numVertices;
    }

  GPUBufferStatic buffer;
  buffer.init();
  buffer.copyDataFloat(bufferData);
  vao.init();
  vao.bind();
  vao.linkBufferAsFloats(buffer, 0 * sizeof(float), 5 * sizeof(float), 3, 0, false);
  vao.linkBufferAsFloats(buffer, 3 * sizeof(float), 5 * sizeof(float), 3, 1, false);
  vao.linkBufferAsFloats(buffer, 6 * sizeof(float), 6 * sizeof(float), 2, 2, false);
  vao.unbind();
  buffer.cleanUp();

  return true;
  }

void MeshStorage::cleanUp()
  {
  vao.cleanUp();
  numVertices = 0;
  }
