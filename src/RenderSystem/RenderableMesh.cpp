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
    clearGLErrors();
    meshStorage->getVAO().bind();
    renderContext->activateShaderProgram(shaderProgram);
    shaderProgram->setVarInt("inLightShaded", lightShaded ? 1 : 0, true);
    shaderProgram->setVarInt("inUseSingleColour", 1, true);
    shaderProgram->setVarVec3("inColour", colour, true);
    shaderProgram->setVarFloat("inAlpha", clampf(1.0f - transparency, 0, 1), true);
    setDepthTest(true);
    setAlphaBlending(transparency > 0.00001f);
    setFaceCulling(backFaceCulling);
    if (wireframeMode)
      {
      glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
      glLineWidth(1);
      }
    glDrawArrays(GL_TRIANGLES, 0, meshStorage->getNumVertices());
    if (wireframeMode)
      glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    ASSERT_NO_GL_ERROR();
    }
  }
