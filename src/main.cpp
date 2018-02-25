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