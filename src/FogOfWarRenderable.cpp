//
// Created by matt on 8/04/18.
//

#include "Resources.h"
#include "FogOfWarRenderable.h"
#include "TOGameContext.h"

FogOfWarRenderable::FogOfWarRenderable(uint id, float fogQuadWidth) : Renderable(id, DRAW_STAGE_FOGOFWAR), fogQuadWidth(fogQuadWidth)
  {}

void FogOfWarRenderable::initialise(RenderContext* renderContext)
  {
  using namespace mathernogl;
  clearGLErrors();
  std::vector<Shader> shaders = { Shader(GL_VERTEX_SHADER, "shaders/FOVTextureVS.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/FOVTextureFS.glsl") };
  shaderProgram = renderContext->getSharedShaderProgram(&shaders);

  float halfWidth = fogQuadWidth * 0.5f;
  GPUBufferStatic vbo;
  vbo.init();
  vbo.bind();
  vbo.copyDataFloat(
    {
    -halfWidth, -halfWidth,
    halfWidth, -halfWidth,
    halfWidth, halfWidth,
    -halfWidth, -halfWidth,
    halfWidth, halfWidth,
    -halfWidth, halfWidth,
    });

  vao.init();
  vao.bind();
  vao.linkBufferAsFloats(vbo, 2, 0, false);
  vao.unbind();
  vbo.cleanUp();

  ASSERT_NO_GL_ERROR();
  }

void FogOfWarRenderable::cleanUp(RenderContext* renderContext)
  {
  vao.cleanUp();
  }

void FogOfWarRenderable::render(RenderContext* renderContext)
  {
  if (!isFogVisible)
    return;

  mathernogl::clearGLErrors();
  renderContext->activateShaderProgram(shaderProgram);
  int loc = renderContext->bindTexture(worldVisibilityTexture);
  shaderProgram->setVarInt("inVisibilityTexture", loc);
  shaderProgram->setVarFloat("inVisibilityWidth", worldVisibilityWidth);
  shaderProgram->setVarFloat("inVisibilityHeight", worldVisibilityHeight);
  vao.bind();
  mathernogl::setDepthTest(false);
  mathernogl::setAlphaBlending(true);
  mathernogl::setFaceCulling(false);
  glDrawArrays(GL_TRIANGLES, 0, 6);
  ASSERT_NO_GL_ERROR();
  }

void FogOfWarRenderable::setWorldVisibilityTexture(TexturePtr texture, float worldWidth, float worldHeight)
  {
  worldVisibilityTexture = texture;
  worldVisibilityWidth = worldWidth;
  worldVisibilityHeight = worldHeight;
  }
