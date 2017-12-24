//
// Created by matherno on 19/11/17.
//

#include "MeshStorage.h"

using namespace mathernogl;

bool MeshStorage::initialiseVAO()
  {
  numVertices = 0;
  if (vertices.empty() || normals.empty())
    return false;

  std::vector<int>* indicesPtr = &indices;
  std::vector<int> tempIndices;
  if (useIndices)
    {
    if (indices.empty())
      return false;
    }
  else
    {
    int index = 0;
    for (const Vector3D& vert : vertices)
      tempIndices.push_back(index++);
    indicesPtr = &tempIndices;
    }

  if (!texCoords.empty())
    return initVAOWithTexCoords(indicesPtr);
  else
    return initVAOWithColours(indicesPtr);
  }

void MeshStorage::cleanUp()
  {
  vao.cleanUp();
  numVertices = 0;
  }

bool MeshStorage::initVAOWithTexCoords(std::vector<int>* indicesPtr)
  {
  std::vector<float> bufferData;
  for (const int& index : *indicesPtr)
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
  }

bool MeshStorage::initVAOWithColours(std::vector<int>* indicesPtr)
  {
  std::vector<float> bufferData;
  for (const int& index : *indicesPtr)
    {
    Vector3D& vert = vertices[index];
    Vector3D& norm = normals[index];
    Vector3D col(0.8, 0.2, 0.6);
    if (!colours.empty())
      col = colours[index];
    bufferData.emplace_back(vert.x);
    bufferData.emplace_back(vert.y);
    bufferData.emplace_back(vert.z);
    bufferData.emplace_back(norm.x);
    bufferData.emplace_back(norm.y);
    bufferData.emplace_back(norm.z);
    bufferData.emplace_back(col.x);
    bufferData.emplace_back(col.y);
    bufferData.emplace_back(col.z);
    ++numVertices;
    }

  GPUBufferStatic buffer;
  buffer.init();
  buffer.copyDataFloat(bufferData);
  vao.init();
  vao.bind();
  vao.linkBufferAsFloats(buffer, 0 * sizeof(float), 6 * sizeof(float), 3, 0, false);
  vao.linkBufferAsFloats(buffer, 3 * sizeof(float), 6 * sizeof(float), 3, 1, false);
  vao.linkBufferAsFloats(buffer, 6 * sizeof(float), 6 * sizeof(float), 3, 2, false);
  vao.unbind();
  buffer.cleanUp();
  }
