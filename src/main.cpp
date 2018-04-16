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
      TOGameContext gameContext(renderContext, loadedState);
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
      renderContext->clearCaches();
      }
    }

  renderContext->cleanUp();
  mathernogl::logInfo("Quiting...");
  return 0;
  }