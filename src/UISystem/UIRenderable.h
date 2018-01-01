#pragma once

#include <src/RenderSystem/RenderSystem.h>

/*
*   
*/

class UIRenderable : public Renderable
  {
private:
  ShaderProgramPtr shaderProgram;
  mathernogl::VertexArray vao;
  mathernogl::GPUBufferStatic vertBuffer;
  Vector3D colour;
  TexturePtr texture;
  bool visible = true;

public:
  UIRenderable(uint id);

  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;

  void setVisible(bool visible){ this->visible = visible; }
  void refresh(const Vector2D& position, const Vector2D& size);
  void setColour(const Vector3D& colour){ this->colour = colour; };
  void setTexture(TexturePtr texture){ this->texture = texture; };
  };
