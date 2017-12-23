#pragma once
//
// Created by matherno on 19/11/17.
//


#include <mathernogl/MathernoGL.h>
#include "RenderSystem.h"
#include "MeshStorage.h"

class RenderableMesh : public Renderable
  {
private:
  MeshStoragePtr meshStorage;
  ShaderProgramPtr shaderProgram;
  bool lightShaded = true;
  Vector3D colour = Vector3D(0.2, 0.5, 0.6);

public:
  RenderableMesh(uint id);

  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;

  void setMeshStorage(MeshStoragePtr meshStorage) { this->meshStorage = meshStorage; }
  MeshStoragePtr getMeshStorage(MeshStoragePtr meshStorage) const { return meshStorage; }
  void setColour(Vector3D colour){ this->colour = colour; }

  void setLightShaded(bool lightShaded){ this->lightShaded = lightShaded; }
  };
