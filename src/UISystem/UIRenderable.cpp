//
// Created by matt on 28/12/17.
//

#include "UIRenderable.h"

using namespace mathernogl;

UIRenderable::UIRenderable(uint id) : Renderable(id)
  {}

void UIRenderable::initialise(RenderContext* renderContext)
  {
  std::vector<Shader> shaders = { Shader(GL_VERTEX_SHADER, "shaders/UserInterfaceVS.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/UserInterfaceFS.glsl") };
  shaderProgram = renderContext->createShaderProgram(&shaders);

  vertBuffer.init();
  vao.init();
  vao.bind();
  vao.linkBufferAsFloats(vertBuffer, 2, 0, false);

  mathernogl::GPUBufferStatic texCoordBuffer;
  texCoordBuffer.init();
  texCoordBuffer.copyDataFloat({ 0,1, 1,1, 1,0, 0,1, 0,0, 1,0});
  vao.linkBufferAsFloats(texCoordBuffer, 2, 1, false);
  vao.unbind();
  texCoordBuffer.cleanUp();
  }

void UIRenderable::cleanUp(RenderContext* renderContext)
  {
  vao.cleanUp();
  vertBuffer.cleanUp();
  shaderProgram->cleanUp();
  }

void UIRenderable::render(RenderContext* renderContext)
  {
  if (!visible)
    return;

  vao.bind();
  renderContext->activateShaderProgram(shaderProgram);
  setFaceCulling(false);
  setDepthTest(true);
  setAlphaBlending(true);
  if (texture)
    {
    shaderProgram->setVarInt("inUseSolidColour", 0);
    shaderProgram->setVarInt("inTexture", renderContext->bindTexture(texture));
    }
  else
    {
    shaderProgram->setVarInt("inUseSolidColour", 1);
    shaderProgram->setVarVec3("inColour", colour);
    }
  clearGLErrors();
  glDrawArrays(GL_TRIANGLES, 0, 6);
  ASSERT_NO_GL_ERROR();
  }

void UIRenderable::refresh(const Vector2D& position, const Vector2D& size)
  {
  vertBuffer.copyDataFloat(
    {
      position.x,           position.y,
      position.x + size.x,  position.y,
      position.x + size.x,  position.y + size.y,
      position.x,           position.y,
      position.x,           position.y + size.y,
      position.x + size.x,  position.y + size.y,
    });
  }
