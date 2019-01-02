//
// Created by matt on 23/12/18.
//

#include "RenderableVoxels.h"

RenderableVoxels::RenderableVoxels(uint id, int drawStage) : Renderable(id, drawStage)
  {

  }

void RenderableVoxels::initialise(RenderContext* renderContext)
  {
  using namespace mathernogl;
  std::vector<Shader> shaders = { Shader(GL_VERTEX_SHADER, "shaders/VoxelVS.glsl"), Shader(GL_GEOMETRY_SHADER, "shaders/VoxelGS.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/VoxelFS.glsl") };
  shaderProgram = renderContext->getSharedShaderProgram(&shaders);
  }

void RenderableVoxels::cleanUp(RenderContext* renderContext)
  {

  }

void RenderableVoxels::render(RenderContext* renderContext)
  {
  mathernogl::clearGLErrors();
  renderContext->activateShaderProgram(shaderProgram);
  mathernogl::setFaceCulling(false);

  int colIdx = 0;
  for (Vector3D col : storage->colours)
    shaderProgram->setVarVec3Array("inColours", colIdx++, col);
  shaderProgram->setVarFloat("inVoxelSize", voxelSize);

  storage->getVAO().bind();
  glDrawArrays(GL_POINTS, 0, storage->getNumVoxels());
  ASSERT_NO_GL_ERROR();
  }

void RenderableVoxels::renderShadowMap(RenderContext* renderContext)
  {
  render(renderContext);
  }

void RenderableVoxels::setVoxelStorage(VoxelStoragePtr storage)
  {
  this->storage = storage;
  }

void RenderableVoxels::setVoxelSize(double size)
  {
  voxelSize = size;
  }

