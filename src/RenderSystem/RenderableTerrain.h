#pragma once

#include "RenderSystem.h"

/*
*   
*/

class RenderableTerrain : public Renderable
  {
private:
  uint numCells;
  float cellSize;
  MeshStoragePtr meshStorage;
  ShaderProgramPtr shaderProgram;
  Vector3D baseColour1;
  Vector3D baseColour2;

public:
  RenderableTerrain(uint id, uint numCells, float cellSize);

  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;

  void setSingleColour(const Vector3D& colour);
  void setMultiColour(const Vector3D& baseColour1, const Vector3D& baseColour2);
  };
