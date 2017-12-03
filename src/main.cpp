#include <iostream>
#include <src/RenderSystem/RenderSystem.h>
#include <src/RenderSystem/RenderContextImpl.h>
#include <src/RenderSystem/RenderableMesh.h>
#include <src/GameSystem/GameContextImpl.h>
#include "TestActor.h"
#include "TestInputHandler.h"
#include "PanCameraInputHandler.h"

int main()
  {
  GameContextImpl context;
  context.initialise();

  PanCameraInputHandler* handler = new PanCameraInputHandler(context.getInputManager()->getNextHandlerID(), Vector3D(0, 20, 30), 0, 0, -20);
  context.addInputHandler(InputHandlerPtr(handler));

  TestInputHandler* handler2 = new TestInputHandler(context.getInputManager()->getNextHandlerID());
  context.addInputHandler(InputHandlerPtr(handler2));

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