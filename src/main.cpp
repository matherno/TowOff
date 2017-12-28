#include <iostream>
#include <src/RenderSystem/RenderSystem.h>
#include <src/RenderSystem/RenderContextImpl.h>
#include <src/RenderSystem/RenderableMesh.h>
#include <src/GameSystem/GameContextImpl.h>
#include <src/ParticleSystem/ParticleEmitter.h>
#include <src/ParticleSystem/RenderableParticles.h>
#include <src/ParticleSystem/ParticleSystem.h>
#include <src/UISystem/UIPanel.h>
#include "TestActor.h"
#include "TestInputHandler.h"
#include "PanCameraInputHandler.h"
#include "TOGameContext.h"

using namespace mathernogl;

int main()
  {
  TOGameContext context;
  context.initialise();

  PanCameraInputHandler* handler = new PanCameraInputHandler(context.getInputManager()->getNextHandlerID(), Vector3D(0, 60, 30), 0, 0, -45);
  context.addInputHandler(InputHandlerPtr(handler));

  UIManager* uiManager = context.getUIManager();
  UIPanel* panel = new UIPanel(uiManager->getNextComponentID());
  panel->setOffset(Vector2D(0, 0));
  panel->setSize(Vector2D(300, 170));
  panel->setBackgroundColour(Vector3D(0, 0, 0.5));
  panel->setHorizontalAlignment(alignmentEnd);
  panel->setVerticalAlignment(alignmentEnd);
//  panel->setHeightMatchParent(true);
  panel->setWidthMatchParent(true);
  uiManager->addComponent(UIComponentPtr(panel));

  {
    UIPanel* button = new UIPanel(uiManager->getNextComponentID());
    button->setOffset(Vector2D(0, 0));
    button->setSize(Vector2D(100, 100));
    button->setBackgroundColour(Vector3D(0, 0.5, 0));
    button->setVerticalAlignment(alignmentCentre);
    button->setHorizontalAlignment(alignmentCentre);
    panel->addChild(UIComponentPtr(button));
  }
  {
    UIPanel* button = new UIPanel(uiManager->getNextComponentID());
    button->setOffset(Vector2D(0, 0));
    button->setSize(Vector2D(100, 100));
    button->setBackgroundColour(Vector3D(0.3, 0, 0));
    button->setVerticalAlignment(alignmentEnd);
    button->setHorizontalAlignment(alignmentEnd);
    panel->addChild(UIComponentPtr(button));
  }
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