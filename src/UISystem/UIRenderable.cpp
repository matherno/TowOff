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
  vao.unbind();
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
  shaderProgram->setVarVec3("inColour", colour);
  glDrawArrays(GL_TRIANGLES, 0, 6);
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
