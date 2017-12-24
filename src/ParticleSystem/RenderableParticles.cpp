//
// Created by matt on 24/12/17.
//

#include "RenderableParticles.h"

using namespace mathernogl;

RenderableParticles::RenderableParticles(uint id) : Renderable(id)
  {}

void RenderableParticles::initialise(RenderContext* renderContext)
  {
  particlesBuffer.init();
  vao.init();
  vao.bind();
  vao.linkBufferAsFloats(particlesBuffer, 0, sizeof(float), 3, 0, false);
  vao.linkBufferAsFloats(particlesBuffer, sizeof(float) * 3, sizeof(float) * 3, 1, 1, false);

  std::vector<Shader> shaders = { Shader(GL_VERTEX_SHADER, "shaders/ParticlesVS.glsl"), Shader(GL_FRAGMENT_SHADER, "shaders/ParticlesFS.glsl") };
  shaderProgram = renderContext->createShaderProgram(&shaders);
  glEnable(GL_PROGRAM_POINT_SIZE);
  }

void RenderableParticles::cleanUp(RenderContext* renderContext)
  {
  shaderProgram->cleanUp();
  }

void RenderableParticles::render(RenderContext* renderContext)
  {
  renderContext->activateShaderProgram(shaderProgram);
  vao.bind();

  int particleCount = 0;
  std::vector<float> posData;
  for(auto iter = emitter->begin(); iter != emitter->end(); iter++)
    {
    ++particleCount;
    Particle* particle = *iter;
    Vector3D pos = particle->position;
    posData.push_back(pos.x);
    posData.push_back(pos.y);
    posData.push_back(pos.z);
    float fade = (float) particle->timeToLive / emitter->getTimeAlive();
    posData.push_back(fade);
    }
  particlesBuffer.copyDataFloat(posData);
  shaderProgram->setVarFloat("inSize", emitter->getParticleSize());
  shaderProgram->setVarVec3("inColour", colour);
  glDrawArrays(GL_POINTS, 0, particleCount);
  }

void RenderableParticles::setEmitter(ParticleEmitterPtr emitter)
  {
  this->emitter = emitter;
  }

void RenderableParticles::setColour(const Vector3D& colour)
  {
  this->colour = colour;
  }
