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

public:
  std::vector<int> indices;
  std::vector<mathernogl::Vector3D> vertices;
  std::vector<mathernogl::Vector3D> normals;
  std::vector<mathernogl::Vector2D> texCoords;

  MeshStorage(const uint id) : id(id) {};
  bool initialiseVAO();
  mathernogl::VertexArray getVAO() { return vao; }
  int getNumVertices() const { return numVertices; }
  void cleanUp();
  };

typedef std::shared_ptr<MeshStorage> MeshStoragePtr;