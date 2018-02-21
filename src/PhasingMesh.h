#pragma once

#include <src/RenderSystem/RenderableMesh.h>

/*
*   
*/

class PhasingMesh : public RenderableMesh
  {
private:
  float phaseSpeed = 0.005f;

public:
  PhasingMesh(uint id);

  virtual void initialise(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;
  void setPhaseSpeed(float speed) { phaseSpeed = speed; }
  };
