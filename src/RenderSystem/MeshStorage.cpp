//
// Created by matherno on 19/11/17.
//

#include "MeshStorage.h"

#define SHADER_LOCATION_VERT  0
#define SHADER_LOCATION_NORM  1
#define SHADER_LOCATION_COL   2
#define SHADER_LOCATION_TEX   3

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

  mathernogl::clearGLErrors();
  GPUBufferStatic buffer;
  buffer.init();
  buffer.copyDataFloat(bufferData);
  vao.init();
  vao.bind();
  vao.linkBufferAsFloats(buffer, 0 * sizeof(float), 5 * sizeof(float), 3, SHADER_LOCATION_VERT, false);
  vao.linkBufferAsFloats(buffer, 3 * sizeof(float), 5 * sizeof(float), 3, SHADER_LOCATION_NORM, false);
  vao.linkBufferAsFloats(buffer, 6 * sizeof(float), 6 * sizeof(float), 2, SHADER_LOCATION_TEX, false);
  vao.unbind();
  buffer.cleanUp();
  ASSERT_NO_GL_ERROR();

  storingTexCoords = true;
  return glGetError() == GL_NO_ERROR;
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

  mathernogl::clearGLErrors();
  GPUBufferStatic buffer;
  buffer.init();
  buffer.copyDataFloat(bufferData);
  vao.init();
  vao.bind();
  vao.linkBufferAsFloats(buffer, 0 * sizeof(float), 6 * sizeof(float), 3, SHADER_LOCATION_VERT, false);
  vao.linkBufferAsFloats(buffer, 3 * sizeof(float), 6 * sizeof(float), 3, SHADER_LOCATION_NORM, false);
  vao.linkBufferAsFloats(buffer, 6 * sizeof(float), 6 * sizeof(float), 3, SHADER_LOCATION_COL, false);
  vao.unbind();
  buffer.cleanUp();
  ASSERT_NO_GL_ERROR();

  storingTexCoords = false;
  return glGetError() == GL_NO_ERROR;
  }

mathernogl::Vector3D MeshStorage::getMin() const
  {
  return min;
  }

mathernogl::Vector3D MeshStorage::getMax() const
  {
  return max;
  }

void MeshStorage::calculateMinMax()
  {
  bool firstVert = true;
  for (Vector3D& vert : vertices)
    {
    if (firstVert)
      {
      min = vert;
      max = vert;
      firstVert = false;
      }
    else
      {
      min.x = std::min(min.x, vert.x);
      min.y = std::min(min.y, vert.y);
      min.z = std::min(min.z, vert.z);
      max.x = std::max(max.x, vert.x);
      max.y = std::max(max.y, vert.y);
      max.z = std::max(max.z, vert.z);
      }
    }
  }
