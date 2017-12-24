//
// Created by matt on 24/12/17.
//

#include "RenderableTerrain.h"

using namespace mathernogl;

RenderableTerrain::RenderableTerrain(uint id, uint numCells, float cellSize)
    : Renderable(id), numCells(numCells), cellSize(cellSize)
  {

  }

void RenderableTerrain::initialise(RenderContext* renderContext)
  {
  std::vector<Shader> shaders = { Shader(GL_VERTEX_SHADER, "shaders/MeshVS.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/MeshFS.glsl") };
  shaderProgram = renderContext->createShaderProgram(&shaders);
  setDepthTest(true);

  meshStorage = renderContext->createEmptyMeshStorage();
  mathernogl::createGrid(numCells, numCells, cellSize, &meshStorage->vertices);
  Vector3D colour;
  for (int vertNum = 0; vertNum < meshStorage->vertices.size(); ++vertNum)
    {
    if (vertNum % 3 == 0)
      colour = mathernogl::lerpVector3D(baseColour1, baseColour2, mathernogl::RandomGenerator::randomFloat(0, 1));
    meshStorage->normals.emplace_back(0, 1, 0);
    meshStorage->colours.emplace_back(colour);
    }
  meshStorage->setUseIndices(false);
  meshStorage->initialiseVAO();
  }

void RenderableTerrain::cleanUp(RenderContext* renderContext)
  {
  shaderProgram->cleanUp();
  }

void RenderableTerrain::render(RenderContext* renderContext)
  {
  renderContext->activateShaderProgram(shaderProgram);
  shaderProgram->setVarInt("inUseSingleColour", 0);
  meshStorage->getVAO().bind();
  glDrawArrays(GL_TRIANGLES, 0, meshStorage->getNumVertices());
  }

void RenderableTerrain::setSingleColour(const Vector3D& colour)
  {
  setMultiColour(colour, colour);
  }

void RenderableTerrain::setMultiColour(const Vector3D& baseColour1, const Vector3D& baseColour2)
  {
  this->baseColour1 = baseColour1;
  this->baseColour2 = baseColour2;
  }

