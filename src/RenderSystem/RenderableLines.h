#pragma once

#include "RenderSystem.h"

/*
*   
*/

class RenderableLines : public Renderable
  {
private:
  struct Line
    {
    Vector3D start;
    Vector3D end;
    Vector3D colour;
    float alpha;
    };

  ShaderProgramPtr shaderProgram;
  mathernogl::VertexArray vao;
  mathernogl::GPUBufferStatic vbo;
  mathernogl::MappedList<Line> lines;
  uint nextLineID = 0;

public:
  RenderableLines(uint id);

  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;

  uint addLine(const Vector3D& start, const Vector3D& end, const Vector3D& colour, float alpha = 1.0f);
  void removeLine(uint lineID);

protected:
  void rebuildBuffer();
  };
