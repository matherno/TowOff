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
  for (GameActorPtr actor : *actors.getList())
    actor->onDetached(this);
  actors.clear();
  actorsToRemove.clear();
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
  if (stage != stageUpdate && stage != stageCleanUp)
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
  if (!paused)
    {
    stage = stageUpdate;
    for (GameActorPtr actor : *actors.getList())
      actor->onUpdate(this);
    stage = stageNone;
    removePendingActors();
    }
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
  if (!paused)
    gameTime += deltaTime;
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

void GameContextImpl::setPaused(bool pause)
  {
  paused = pause;
  }

bool GameContextImpl::isPaused() const
  {
  return paused;
  }

Vector3D GameContextImpl::getCursorWorldPos(uint cursorX, uint cursorY)
  {
  uint screenWidth = getRenderContext()->getWindow()->getWidth();
  uint screenHeight = getRenderContext()->getWindow()->getHeight();
  Vector3D clipSpacePos = Vector3D((float)cursorX / (float)screenWidth * 2.0f - 1.0f, (1.0f - (float)cursorY / (float)screenHeight) * 2.0f - 1.0f, 0);
  return clipToWorldTransform(clipSpacePos);
  }

Vector3D GameContextImpl::getViewDirection()
  {
  return (clipToWorldTransform(Vector3D(0, 0, 1)) - clipToWorldTransform(Vector3D(0, 0, 0))).getUniform();
  }

Vector3D GameContextImpl::getViewDirectionAtCursor(uint cursorX, uint cursorY)
  {
  uint screenWidth = getRenderContext()->getWindow()->getWidth();
  uint screenHeight = getRenderContext()->getWindow()->getHeight();
  Vector3D clipSpacePos = Vector3D((float)cursorX / (float)screenWidth * 2.0f - 1.0f, (1.0f - (float)cursorY / (float)screenHeight) * 2.0f - 1.0f, 0);
  return (clipToWorldTransform(clipSpacePos + Vector3D(0, 0, 1)) - clipToWorldTransform(clipSpacePos)).getUniform();
  }

Vector3D GameContextImpl::clipToWorldTransform(const Vector3D& clipSpacePos)
  {
  using namespace mathernogl;
  return clipSpacePos * matrixInverse(*getRenderContext()->getCameraToClip()) * matrixInverse(*getRenderContext()->getWorldToCamera());
  }

