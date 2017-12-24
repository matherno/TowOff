#pragma once
//
// Created by matherno on 19/11/17.
//

#include <mathernogl/MathernoGL.h>

class MeshStorage
  {
private:
  const uint id;
  mathernogl::VertexArray vao;
  int numVertices = 0;
  bool useIndices = true;

public:
  std::vector<int> indices;
  std::vector<mathernogl::Vector3D> vertices;
  std::vector<mathernogl::Vector3D> normals;
  std::vector<mathernogl::Vector2D> texCoords;
  std::vector<mathernogl::Vector3D> colours;

  MeshStorage(const uint id) : id(id) {};
  bool initialiseVAO();
  mathernogl::VertexArray getVAO() { return vao; }
  int getNumVertices() const { return numVertices; }
  void cleanUp();
  void setUseIndices(bool use){ useIndices = use; }

protected:
  bool initVAOWithTexCoords(std::vector<int>* indicesPtr);
  bool initVAOWithColours(std::vector<int>* indicesPtr);
  };

typedef std::shared_ptr<MeshStorage> MeshStoragePtr;