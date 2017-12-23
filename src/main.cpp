#include <iostream>
#include <src/RenderSystem/RenderSystem.h>
#include <src/RenderSystem/RenderContextImpl.h>
#include <src/RenderSystem/RenderableMesh.h>
#include <src/GameSystem/GameContextImpl.h>
#include "TestActor.h"
#include "TestInputHandler.h"
#include "PanCameraInputHandler.h"
#include "TOGameContext.h"

int main()
  {
  TOGameContext context;
  context.initialise();

  PanCameraInputHandler* handler = new PanCameraInputHandler(context.getInputManager()->getNextHandlerID(), Vector3D(0, 60, 30), 0, 0, -45);
  context.addInputHandler(InputHandlerPtr(handler));

  mathernogl::RandomGenerator::setSeed();
  for (int num = 0; num < 50; ++num)
    {
    TowerPtr tower = context.createBasicTower(Vector3D(mathernogl::RandomGenerator::randomFloat(-40, 40), 0, mathernogl::RandomGenerator::randomFloat(-40, 40)));
    tower->setPlayerNum((uint)mathernogl::RandomGenerator::randomInt(1, 2));
    }

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