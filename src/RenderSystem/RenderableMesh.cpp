//
// Created by matherno on 19/11/17.
//

#include "RenderableMesh.h"

using namespace mathernogl;

RenderableMesh::RenderableMesh(uint id) : Renderable(id)
  {}

void RenderableMesh::initialise(RenderContext* renderContext)
  {
  std::vector<Shader> shaders = { Shader(GL_VERTEX_SHADER, "shaders/MeshVS.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/MeshFS.glsl") };
  shaderProgram = renderContext->createShaderProgram(&shaders);
  }

void RenderableMesh::cleanUp(RenderContext* renderContext)
  {
  shaderProgram->cleanUp();
  }

void RenderableMesh::render(RenderContext* renderContext)
  {
  if (meshStorage)
    {
    meshStorage->getVAO().bind();
    renderContext->activateShaderProgram(shaderProgram);
    shaderProgram->setVarInt("inLightShaded", lightShaded ? 1 : 0);
    shaderProgram->setVarInt("inUseSingleColour", 1);
    shaderProgram->setVarVec3("inColour", colour);
    setDepthTest(true);
    setAlphaBlending(true);
    setFaceCulling(true);
    glDrawArrays(GL_TRIANGLES, 0, meshStorage->getNumVertices());
    }
  }
