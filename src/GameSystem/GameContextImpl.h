#pragma once

#include <list>
#include <map>
#include <mathernogl/MathernoGL.h>
#include "GameSystem.h"
#include "InputManagerImpl.h"
#include "Camera.h"
#include <src/RenderSystem/RenderContextImpl.h>

class GameContextImpl : public GameContext
  {
private:
  InputManagerImpl inputManager;
  RenderContextImpl renderContext;
  uint nextActorID = 1;
  mathernogl::MappedList<GameActorPtr> actors;
  Camera camera;
  long startFrameTime;
  long deltaTime = 0;

public:
  GameContextImpl() {}
  virtual ~GameContextImpl() override {}

  virtual bool initialise() override;
  virtual void cleanUp() override;

  virtual void addActor(GameActorPtr actor) override;
  virtual void removeActor(uint id) override;
  virtual int getNumActors() override;
  virtual bool gotActor(uint id) override;
  virtual uint getNextActorID() override;

  virtual void addInputHandler(InputHandlerPtr handler) override;
  virtual void removeInputHandler(InputHandlerPtr handler) override;

  virtual long getDeltaTime() override { return deltaTime; }
  virtual void startFrame() override;
  virtual void processInputStage() override;
  virtual void processUpdateStage() override;
  virtual void processDrawStage() override;
  virtual void endFrame(float maxFPS) override;

  virtual Camera* getCamera() override { return &camera; }
  virtual InputManager* getInputManager() override { return &inputManager; }
  virtual RenderContext* getRenderContext() override { return &renderContext; }
  };
