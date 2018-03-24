//
// Created by matherno on 19/11/17.
//

#include "RenderableMesh.h"

using namespace mathernogl;

RenderableMesh::RenderableMesh(uint id) : Renderable(id)
  {
  drawStyle = MESH_DRAW_STYLE_SINGLE_COLOUR;
  }

void RenderableMesh::initialise(RenderContext* renderContext)
  {
  std::vector<Shader> shaders = { Shader(GL_VERTEX_SHADER, "shaders/MeshVS.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/MeshFS.glsl") };
  shaderProgram = renderContext->getSharedShaderProgram(&shaders);
  }

void RenderableMesh::cleanUp(RenderContext* renderContext)
  {
  }

void RenderableMesh::render(RenderContext* renderContext)
  {
  if (meshStorage)
    {
    clearGLErrors();
    meshStorage->getVAO().bind();

    renderContext->activateShaderProgram(shaderProgram);
    shaderProgram->setVarInt("inLightShaded", lightShaded ? 1 : 0, true);
    shaderProgram->setVarFloat("inAlpha", clampf(1.0f - transparency, 0, 1), true);

    shaderProgram->setVarInt("inDrawStyle", drawStyle, true);
    if (drawStyle == MESH_DRAW_STYLE_TEXTURE && texture)
      shaderProgram->setVarInt("inTexture", renderContext->bindTexture(texture), true);
    if (drawStyle == MESH_DRAW_STYLE_SINGLE_COLOUR)
      shaderProgram->setVarVec3("inColour", colour, true);

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

void RenderableMesh::setDrawStyleSingleColour(Vector3D colour)
  {
  drawStyle = MESH_DRAW_STYLE_SINGLE_COLOUR;
  texture.reset();
  this->colour = colour;
  }

void RenderableMesh::setDrawStyleTexture(TexturePtr texture)
  {
  drawStyle = MESH_DRAW_STYLE_TEXTURE;
  this->texture = texture;
  }

void RenderableMesh::setDrawStyleVertColours()
  {
  drawStyle = MESH_DRAW_STYLE_VERT_COLOUR;
  texture.reset();
  }
