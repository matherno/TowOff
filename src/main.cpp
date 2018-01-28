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


int main()
  {
  TOGameContext context;
  context.initialise();

  PanCameraInputHandler* handler = new PanCameraInputHandler(context.getInputManager()->getNextHandlerID(), Vector3D(0, 70, 60), 0, 0, -45);
  context.addInputHandler(InputHandlerPtr(handler));

  UIManager* uiManager = context.getUIManager();
  UIPanel* panel = new UIPanel(uiManager->getNextComponentID());
  panel->setOffset(Vector2D(0, 0));
  panel->setSize(Vector2D(300, 170));
  panel->setBackgroundColour(Vector3D(0.4, 0.3, 0.1));
  panel->setHorizontalAlignment(alignmentEnd);
  panel->setVerticalAlignment(alignmentEnd);
//  panel->setHeightMatchParent(true);
  panel->setWidthMatchParent(true);
  uiManager->addComponent(UIComponentPtr(panel));

  UIPanel* subPanel = new UIPanel(uiManager->getNextComponentID());
  subPanel->setOffset(Vector2D(0, 0));
  subPanel->setSize(Vector2D(100, 100));
  subPanel->setBackgroundColour(Vector3D(0.35, 0.35, 0.3));
  subPanel->setVerticalAlignment(alignmentCentre);
  subPanel->setHorizontalAlignment(alignmentCentre);
  subPanel->setHeightMatchParent(true);
  subPanel->setWidthMatchParent(true);
  subPanel->setPadding(15, 10);
  panel->addChild(UIComponentPtr(subPanel));


  {
    UIPanel* button = new UIPanel(uiManager->getNextComponentID());
    button->setOffset(Vector2D(50, 20));
    button->setSize(Vector2D(70, 70));
    button->setBackgroundColour(context.getPlayerColour(1));
    button->setVerticalAlignment(alignmentStart);
    button->setHorizontalAlignment(alignmentStart);
    button->setMouseClickCallback([&handler, &panel, &context](uint x, uint y)->bool
                                    {
                                    handler->setActivePlayer(1);
                                    panel->setBackgroundColour(context.getPlayerColour(1));
                                    return true;
                                    });
    subPanel->addChild(UIComponentPtr(button));
  }

  {
    UIPanel* button = new UIPanel(uiManager->getNextComponentID());
    button->setOffset(Vector2D(150, 20));
    button->setSize(Vector2D(70, 70));
    button->setBackgroundColour(context.getPlayerColour(2));
    button->setVerticalAlignment(alignmentStart);
    button->setHorizontalAlignment(alignmentStart);
    button->setMouseClickCallback([&handler, &panel, &context](uint x, uint y)->bool
                                    {
                                    handler->setActivePlayer(2);
                                    panel->setBackgroundColour(context.getPlayerColour(2));
                                    return true;
                                    });
    subPanel->addChild(UIComponentPtr(button));
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

    if (handler->getActivePlayer() != 0)
      panel->setBackgroundColour(context.getPlayerColour(handler->getActivePlayer()));
    else
      panel->setBackgroundColour(Vector3D(0.4, 0.3, 0.1));
    panel->invalidate();

    }

  context.cleanUp();

  return 0;
  }