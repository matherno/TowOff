//
// Created by matherno on 22/11/17.
//

#include "GameContextImpl.h"

bool GameContextImpl::initialise()
  {
  stage = stageInit;
  RenderInitConfig config;
  config.windowName = "Testing";
  config.windowWidth = 1280;
  config.windowHeight = 768;
  if (!renderContext.initialise(&config))
    {
    mathernogl::logError("Failed to initialise render system!");
    return false;
    }

  inputManager.initialise(renderContext.getWindow());

  startTime = mathernogl::getTimeMS();
  gameTime = 0;

  stage = stageNone;
  return true;
  }

void GameContextImpl::cleanUp()
  {
  stage = stageCleanUp;
  renderContext.cleanUp();
  stage = stageNone;
  }

void GameContextImpl::addActor(GameActorPtr actor)
  {
  actors.add(actor, actor->getID());
  actor->onAttached(this);
  }

void GameContextImpl::removeActor(uint id)
  {
  if (stage != stageUpdate)
    {
    if (actors.contains(id))
      {
      GameActorPtr actor = actors.get(id);
      actors.remove(id);
      actor->onDetached(this);
      }
    }
  else
    {
    actorsToRemove.emplace(id);
    }
  }

int GameContextImpl::getNumActors()
  {
  return actors.count();
  }

bool GameContextImpl::gotActor(uint id)
  {
  return actors.contains(id);
  }

uint GameContextImpl::getNextActorID()
  {
  return nextActorID++;
  }

void GameContextImpl::processInputStage()
  {
  stage = stageInput;
  inputManager.processInput(this);
  stage = stageNone;
  }

void GameContextImpl::processUpdateStage()
  {
  stage = stageUpdate;
  for (GameActorPtr actor : *actors.getList())
    actor->onUpdate(this);
  stage = stageNone;
  removePendingActors();
  }

void GameContextImpl::processDrawStage()
  {
  stage = stageRender;
  if (!camera.isValid())
    {
    renderContext.setWorldToCamera(*camera.getWorldToCamera()->getTransformMatrix());
    renderContext.setCameraToClip(*camera.getCameraToClip()->getTransformMatrix());
    camera.setValid(true);
    }

  renderContext.render();
  stage = stageNone;
  }

void GameContextImpl::addInputHandler(InputHandlerPtr handler)
  {
  inputManager.addHandler(handler);
  handler->onAttached(this);
  }

void GameContextImpl::removeInputHandler(InputHandlerPtr handler)
  {
  inputManager.removeHandler(handler->getID());
  handler->onDetached(this);
  }

void GameContextImpl::startFrame()
  {
  gameTime = mathernogl::getTimeMS() - startTime;
  startFrameTime = mathernogl::getTimeMS();
  }

void GameContextImpl::endFrame(float maxFPS)
  {
  long minTimeBetweenFrames = (long)((1.0 / (double)maxFPS) * 1000);
  while (mathernogl::getTimeMS() - startFrameTime < minTimeBetweenFrames)
    {
    }
  deltaTime = mathernogl::getTimeMS() - startFrameTime;
  }

void GameContextImpl::removePendingActors()
  {
  std::for_each(actorsToRemove.begin(), actorsToRemove.end(), [this](uint id)
    {
    removeActor(id);
    });
  actorsToRemove.clear();
  }

