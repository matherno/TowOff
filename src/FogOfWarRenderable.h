#pragma once

#include <RenderSystem/RenderSystem.h>

/*
*   
*/

class FogOfWarRenderable : public Renderable
  {
private:
  const float fogQuadWidth;
  mathernogl::VertexArray vao;
  ShaderProgramPtr shaderProgram;
  TexturePtr worldVisibilityTexture;
  float worldVisibilityWidth;
  float worldVisibilityHeight;

public:
  FogOfWarRenderable(uint id, float fogQuadWidth);

  //  assumes the texture centre point is at the world origin, spanning the given world width and height
  void setWorldVisibilityTexture(TexturePtr texture, float worldWidth, float worldHeight);
  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;
  };
