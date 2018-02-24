#pragma once
//
// Created by matherno on 19/11/17.
//

#include <mathernogl/MathernoGL.h>

#define DRAW_STYLE_SINGLE_COLOUR 1
#define DRAW_STYLE_TEXTURE       2
#define DRAW_STYLE_VERT_COLOUR   3

class MeshStorage
  {
private:
  const uint id;
  mathernogl::VertexArray vao;
  int numVertices = 0;
  bool useIndices = true;
  mathernogl::Vector3D min;
  mathernogl::Vector3D max;
  bool storingTexCoords = false;

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
  mathernogl::Vector3D getMin() const;
  mathernogl::Vector3D getMax() const;
  void calculateMinMax();
  bool gotTexCoords() const { return storingTexCoords; }

protected:
  bool initVAOWithTexCoords(std::vector<int>* indicesPtr);
  bool initVAOWithColours(std::vector<int>* indicesPtr);
  };

typedef std::shared_ptr<MeshStorage> MeshStoragePtr;