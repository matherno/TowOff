//
// Created by matherno on 22/11/17.
//

#include "GameContextImpl.h"

bool GameContextImpl::initialise()
  {
  RenderInitConfig config;
  config.windowName = "Testing";
  if (!renderContext.initialise(&config))
    {
    mathernogl::logError("Failed to initialise render system!");
    return false;
    }

  inputManager.initialise(renderContext.getWindow());

  return true;
  }

void GameContextImpl::cleanUp()
  {
  renderContext.cleanUp();
  }

void GameContextImpl::addActor(GameActorPtr actor)
  {
  actors.add(actor, actor->getID());
  actor->onAttached(this);
  }

void GameContextImpl::removeActor(uint id)
  {
  if (actors.contains(id))
    {
    GameActorPtr actor = actors.get(id);
    actors.remove(id);
    actor->onDetached(this);
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
  inputManager.processInput(this);
  }

void GameContextImpl::processUpdateStage()
  {
  for (GameActorPtr actor : *actors.getList())
    actor->onUpdate(this);
  }

void GameContextImpl::processDrawStage()
  {
  if (!camera.isValid())
    {
    renderContext.setWorldToCamera(*camera.getWorldToCamera()->getTransformMatrix());
    renderContext.setCameraToClip(*camera.getCameraToClip()->getTransformMatrix());
    camera.setValid(true);
    }

  renderContext.render();
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

