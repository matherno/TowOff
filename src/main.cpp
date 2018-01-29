#include <iostream>
#include <RenderSystem/RenderSystem.h>
#include <RenderSystem/RenderContextImpl.h>
#include <RenderSystem/RenderableMesh.h>
#include <GameSystem/GameContextImpl.h>
#include <ParticleSystem/ParticleEmitter.h>
#include <ParticleSystem/RenderableParticles.h>
#include <ParticleSystem/ParticleSystem.h>
#include <UISystem/UIPanel.h>
#include "TestActor.h"
#include "TestInputHandler.h"
#include "TOInputHandler.h"
#include "TOGameContext.h"


int main()
  {
  TOGameContext context;
  context.initialise();


//  mathernogl::RandomGenerator::setSeed();
//  for (int num = 0; num < 50; ++num)
//    {
//    TowerPtr tower = context.createBasicTower(Vector3D(mathernogl::RandomGenerator::randomFloat(-40, 40), 0, mathernogl::RandomGenerator::randomFloat(-40, 40)));
//    tower->setPlayerNum((uint)mathernogl::RandomGenerator::randomInt(1, 2));
//    }

//  glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

  while(context.getRenderContext()->isWindowOpen())
    {
    context.startFrame();
    context.processInputStage();
    context.processUpdateStage();
    context.processDrawStage();
    context.endFrame(60);
    }

  context.cleanUp();

  return 0;
  }