#pragma once

#include <src/RenderSystem/RenderSystem.h>
#include "ParticleEmitter.h"

/*
*   
*/

class RenderableParticles : public Renderable
  {
private:
  ParticleEmitterPtr emitter;
  ShaderProgramPtr shaderProgram;
  mathernogl::VertexArray vao;
  mathernogl::GPUBufferStatic particlesBuffer;
  Vector3D colour = Vector3D(1, 0, 0);

public:
  RenderableParticles(uint id);

  void setEmitter(ParticleEmitterPtr emitter);

  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;

  void setColour(const Vector3D& colour);
  };
