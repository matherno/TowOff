//
// Created by matherno on 19/11/17.
//

#include "RenderableMesh.h"

using namespace mathernogl;

RenderableMesh::RenderableMesh(uint id, int drawStage) : Renderable(id, drawStage)
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
    shaderProgram->setVarInt("inCellShadeGrades", cellShadedGrades, true);
    shaderProgram->setVarFloat("inAlpha", clampf(1.0f - transparency, 0, 1), true);
    shaderProgram->setVarFloat("inTextureColourMix", textureColourMix, true);

    shaderProgram->setVarInt("inDrawStyle", drawStyle, true);
    if (drawStyle == MESH_DRAW_STYLE_TEXTURE && texture)
      {
      shaderProgram->setVarInt("inTexture", renderContext->bindTexture(texture), true);
      if (textureColourMix > 0)
        shaderProgram->setVarVec3("inColour", colour, true);
      }
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

    MeshStorageInstanced* instancedMeshStorage = dynamic_cast<MeshStorageInstanced*>(meshStorage.get());
    if (instancedMeshStorage)
      {
      //  instanced rendering...
      shaderProgram->setVarInt("inUseInstanceTransforms", 1, true);
      glDrawArraysInstanced(GL_TRIANGLES, 0, instancedMeshStorage->getNumVertices(), instancedMeshStorage->getNumInstances());
      }
    else
      {
      //  singular rendering...
      shaderProgram->setVarInt("inUseInstanceTransforms", 0, true);
      glDrawArrays(GL_TRIANGLES, 0, meshStorage->getNumVertices());
      }

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

void RenderableMesh::setDrawStyleTexture(TexturePtr texture, float colourMixFactor, const Vector3D& colour)
  {
  setDrawStyleTexture(texture);
  this->colour = colour;
  this->textureColourMix = colourMixFactor;
  }

void RenderableMesh::setDrawStyleVertColours()
  {
  drawStyle = MESH_DRAW_STYLE_VERT_COLOUR;
  texture.reset();
  }
