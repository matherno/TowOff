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
#include "TOInputHandler.h"
#include "TOGameContext.h"
#include "TOMainMenuContext.h"

//  essentially an exception type that will never be thrown (to make a try-catch block not catch anything)
class null_exception : public std::exception
  {};

void attemptCrashCleanup(RenderContext* renderContext)
  {
  std::vector<RenderablePtr> renderables;
  renderContext->getRenderableSet()->forEachChild([&renderables](RenderablePtr renderable)
    {
    renderables.push_back(renderable);
    });
  for (RenderablePtr renderable : renderables)
    {
    renderable->cleanUp(renderContext);
    renderContext->getRenderableSet()->removeRenderable(renderable->getID());
    }
  }

void runGame()
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
    return;
    }

  TOMainMenuContext::MainMenuOutcome mainMenuOutcome;
  while (renderContext->isWindowOpen())
    {
    mainMenuOutcome = TOMainMenuContext::doMainMenu(renderContext, settings);
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

      try
        {
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
        }
#ifdef NDEBUG
      catch (const std::exception& err)
#else
      catch (const null_exception& err)   //  no exceptions will be caught (so they can be captured in debug)
#endif
        {
        mathernogl::logInfo("Game crashed: " + string(err.what()));
        attemptCrashCleanup(renderContext.get());
        }

      renderContext->reset();
      }
    }

  renderContext->cleanUp();
  mathernogl::logInfo("Quiting...");
  }

int main()
  {
  try
    {
    runGame();
    }
#ifdef NDEBUG
    catch (const std::exception& err)
#else
  catch (const null_exception& err)   //  no exceptions will be caught (so they can be captured in debug)
#endif
    {
    mathernogl::logInfo("Game completely crashed: " + string(err.what()));
    }
  }