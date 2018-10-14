//
// Created by matt on 19/02/18.
//

#include "PhasingMesh.h"

PhasingMesh::PhasingMesh(uint id, int drawStage) : RenderableMesh(id, drawStage)
  {}

void PhasingMesh::initialise(RenderContext* renderContext)
  {
  RenderableMesh::initialise(renderContext);
  using namespace mathernogl;
  std::vector<Shader> shaders = { Shader(GL_VERTEX_SHADER, "shaders/PhasingMeshVS.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/PhasingMeshFS.glsl") };
  shaderProgram = renderContext->getSharedShaderProgram(&shaders);
  setBackFaceCulling(false);
  }

void PhasingMesh::render(RenderContext* renderContext)
  {
  renderContext->activateShaderProgram(shaderProgram);
  shaderProgram->setVarFloat("inPhaseSpeed", phaseSpeed);
  RenderableMesh::render(renderContext);
  }
