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

int main()
  {
  std::shared_ptr<TOSettings> settings(new TOSettings());
  settings->initialise();

  RenderInitConfig renderConfig;
  renderConfig.windowName = "TowOff";
  renderConfig.windowWidth = (uint)mathernogl::clampi(settings->getWindowWidth(), 800, 2560);
  renderConfig.windowHeight = (uint)mathernogl::clampi(settings->getWindowHeight(), 600, 2440);
  renderConfig.antiAliasing = settings->useAntiAliasing();
  renderConfig.fullscreen = settings->useFullscreen();

  RenderContextPtr renderContext(new RenderContextImpl());
  if (!renderContext->initialise(&renderConfig))
    {
    mathernogl::logError("Failed to initialise render system!");
    return 0;
    }

  TOMainMenuContext::MainMenuOutcome mainMenuOutcome;
  while (renderContext->isWindowOpen())
    {
    mainMenuOutcome = TOMainMenuContext::doMainMenu(renderContext);
    if (mainMenuOutcome.isQuitGame())
      break;

    std::shared_ptr<TOGameState> loadedState;
    if (mainMenuOutcome.isLoadGame())
      {
      loadedState = mainMenuOutcome.loadedState;
      if (!loadedState)
        continue;
      }

    if (mainMenuOutcome.isNewGame() || mainMenuOutcome.isLoadGame())
      {
      mathernogl::logInfo("Starting game...");
      TOGameContext gameContext(renderContext, settings, loadedState);
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
      renderContext->reset();
      }
    }

  renderContext->cleanUp();
  mathernogl::logInfo("Quiting...");
  return 0;
  }