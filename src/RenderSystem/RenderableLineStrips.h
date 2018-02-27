#pragma once

#include "RenderSystem.h"

/*
*   
*/

class RenderableLineStrips : public Renderable
  {
private:
  struct LineStrip
    {
    std::vector<Vector3D> points;
    Vector3D colour;
    float alpha;
    };

  ShaderProgramPtr shaderProgram;
  mathernogl::VertexArray vao;
  mathernogl::GPUBufferStatic vbo;
  mathernogl::MappedList<LineStrip> lineStrips;
  uint nextLineStripID = 1;
  bool rebuildDisabled = false;
  LineStrip pendingLineStrip;
  uint lineWidth = 2;

public:
  RenderableLineStrips(uint id);

  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;

  void setLineWidth(uint width) { lineWidth = width; }
  void startLineStrip(const Vector3D& colour, float alpha = 1.0f);
  void addXZPlaneCircle(const Vector3D& centre, float radius, uint numSectors = 30);
  void addPoint(const Vector3D& point);
  uint finishLineStrip();
  void removeLineStrip(uint lineStripID);
  void clearLineStrips();

  void enableRebuild(){ rebuildDisabled = false; }
  void disableRebuild(){ rebuildDisabled = true; }
  void rebuildBuffer();
  };
