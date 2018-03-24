#include <iostream>
#include <RenderSystem/RenderSystem.h>
#include <RenderSystem/RenderContextImpl.h>
#include <RenderSystem/RenderableMesh.h>
#include <GameSystem/GameContextImpl.h>
#include <ParticleSystem/ParticleEmitter.h>
#include <ParticleSystem/RenderableParticles.h>
#include <ParticleSystem/ParticleSystem.h>
#include <UISystem/UIPanel.h>
#include <RenderSystem/RenderableLineStrips.h>
#include "TestActor.h"
#include "TestInputHandler.h"
#include "TOInputHandler.h"
#include "TOGameContext.h"
#include "TOMainMenuContext.h"

TOMainMenuContext::Option doMainMenu(RenderContextPtr renderContext)
  {
  TOMainMenuContext mainMenuContext(renderContext);
  mainMenuContext.initialise();
  while(!mainMenuContext.isContextEnded() && renderContext->isWindowOpen())
    {
    mainMenuContext.startFrame();
    mainMenuContext.processInputStage();
    mainMenuContext.processUpdateStage();
    mainMenuContext.processDrawStage();
    mainMenuContext.endFrame(30);
    }
  TOMainMenuContext::Option outcome = mainMenuContext.getSelectedOption();
  mainMenuContext.cleanUp();
  return outcome;
  }

int main()
  {
  RenderInitConfig renderConfig;
  renderConfig.windowName = "TowOff";
  renderConfig.windowWidth = 1800;
  renderConfig.windowHeight = 900;
  renderConfig.antiAliasing = true;

  RenderContextPtr renderContext(new RenderContextImpl());
  if (!renderContext->initialise(&renderConfig))
    {
    mathernogl::logError("Failed to initialise render system!");
    return 0;
    }

  TOMainMenuContext::Option mainMenuOutcome = doMainMenu(renderContext);

  while (mainMenuOutcome != TOMainMenuContext::optionNone && mainMenuOutcome != TOMainMenuContext::optionQuit && renderContext->isWindowOpen())
    {
    TOGameContext gameContext(renderContext);
    gameContext.initialise();
    while (!gameContext.isContextEnded() && renderContext->isWindowOpen())
      {
      gameContext.startFrame();
      gameContext.processInputStage();
      gameContext.processUpdateStage();
      gameContext.processDrawStage();
      gameContext.endFrame(60);
      }
    gameContext.cleanUp();

    mainMenuOutcome = doMainMenu(renderContext);
    }

  renderContext->cleanUp();

  return 0;
  }