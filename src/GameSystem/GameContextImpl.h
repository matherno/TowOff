#pragma once

#include <list>
#include <map>
#include <mathernogl/MathernoGL.h>
#include "GameSystem.h"
#include "InputManagerImpl.h"
#include "Camera.h"
#include <RenderSystem/RenderContextImpl.h>
#include <set>
#include <UISystem/UIManagerImpl.h>

class GameContextImpl : public GameContext
  {
enum ProcessStage
  {
  stageNone,
  stageInit,
  stageInput,
  stageUpdate,
  stageRender,
  stageCleanUp,
  };

private:
  InputManagerImpl inputManager;
  RenderContextImpl renderContext;
  UIManagerImpl uiManager;
  uint nextActorID = 1;
  mathernogl::MappedList<GameActorPtr> actors;
  Camera camera;
  long startFrameTime;
  long gameTime = 0;
  long deltaTime = 0;
  ProcessStage stage = stageNone;
  std::set<uint> actorsToRemove;
  bool paused = false;

protected:
  RenderInitConfig renderConfig;

public:
  GameContextImpl();
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

  virtual long getGameTime() override { return gameTime; }
  virtual long getDeltaTime() override { return deltaTime; }
  virtual void startFrame() override;
  virtual void processInputStage() override;
  virtual void processUpdateStage() override;
  virtual void processDrawStage() override;
  virtual void endFrame(float maxFPS) override;
  virtual void setPaused(bool pause) override;

  virtual bool isPaused() const override;

  virtual Camera* getCamera() override { return &camera; }
  virtual InputManager* getInputManager() override { return &inputManager; }
  virtual RenderContext* getRenderContext() override { return &renderContext; }
  virtual UIManager* getUIManager() override { return &uiManager; }
  virtual const Camera* getCamera() const override { return &camera; }
  virtual const InputManager* getInputManager() const override { return &inputManager; }
  virtual const RenderContext* getRenderContext() const override { return &renderContext; }
  virtual const UIManager* getUIManager() const override { return &uiManager; }

  Vector3D getCursorWorldPos(uint cursorX, uint cursorY) const;
  Vector3D clipToWorldTransform(const Vector3D& clipSpacePos) const;
  Vector3D getViewDirection() const;
  Vector3D getViewDirectionAtCursor(uint cursorX, uint cursorY) const;

protected:
  void removePendingActors();
  };
