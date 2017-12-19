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

  PanCameraInputHandler* handler = new PanCameraInputHandler(context.getInputManager()->getNextHandlerID(), Vector3D(0, 20, 30), 0, 0, -20);
  context.addInputHandler(InputHandlerPtr(handler));

  for (int num = 0; num < 20; ++num)
    {
    context.createBasicTower(Vector3D(mathernogl::RandomGenerator::randomFloat(-40, 40), 0, mathernogl::RandomGenerator::randomFloat(-40, 40)));
    }
//  context.createBasicTower(Vector3D(0, 0, 0));
//  context.createBasicTower(Vector3D(20, 0, 0));
//  context.createBasicTower(Vector3D(20, 0, 10));

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