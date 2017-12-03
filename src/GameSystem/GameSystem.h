#pragma once

#include <memory>
#include <src/RenderSystem/RenderSystem.h>
#include "Camera.h"

class GameContext;

/*
*   GameActor, an actor in the game representing an object with some behaviour and/or visual presence in the scene
*/
class GameActor
  {
private:
  const uint id;
public:
  GameActor(uint id) : id(id) {}
  virtual ~GameActor() {}
  uint getID(){ return id; }
  virtual void onAttached(GameContext* gameContext) = 0;
  virtual void onUpdate(GameContext* gameContext) = 0;
  virtual void onDetached(GameContext* gameContext) = 0;
  };
typedef std::shared_ptr<GameActor> GameActorPtr;


/*
*   InputHandler, an object ready to receive input from the player, and perform actions based on them
*       event functions return boolean, which indicates whether or not they handled and consumed the event
*/
class InputHandler
  {
private:
  const uint id;
public:
  InputHandler(uint id): id(id) {}
  virtual ~InputHandler() {}
  uint getID() { return id; }
  virtual void onAttached(GameContext* gameContext) = 0;
  virtual void onDetached(GameContext* gameContext) = 0;
  virtual bool onMousePressed(GameContext* gameContext, uint button, uint mouseX, uint mouseY) { return false; }
  virtual bool onMouseHeld(GameContext* gameContext, uint button, uint mouseX, uint mouseY) { return false; }
  virtual bool onMouseReleased(GameContext* gameContext, uint button, uint mouseX, uint mouseY) { return false; }
  virtual bool onMouseScroll(GameContext* gameContext, double scrollOffset, uint mouseX, uint mouseY) { return false; }
  virtual bool onMouseMove(GameContext* gameContext, uint mouseX, uint mouseY, uint prevMouseX, uint prevMouseY) { return false; }
  virtual bool onKeyPressed(GameContext* gameContext, uint key) { return false; }
  virtual bool onKeyHeld(GameContext* gameContext, uint key) { return false; }
  virtual bool onKeyReleased(GameContext* gameContext, uint key) { return false; }
  };
typedef std::shared_ptr<InputHandler> InputHandlerPtr;

/*
*   InputManager, responsible for managing a bunch of input handlers, and calling their event functions
*/
class InputManager
  {
public:
  InputManager(){}
  virtual ~InputManager(){}
  virtual void initialise(mathernogl::Window* window) = 0;
  virtual void cleanUp() = 0;
  virtual uint getNextHandlerID() = 0;
  virtual void containsHandler(uint id) = 0;
protected:
  virtual void addHandler(InputHandlerPtr handler) = 0;
  virtual void removeHandler(uint id) = 0;
  virtual void processInput(GameContext* gameContext) = 0;
  };

/*
*   GameContext, the centralised game state and controller
*/
class GameContext
  {
public:
  GameContext(){};
  virtual ~GameContext(){};
  virtual bool initialise() = 0;
  virtual void cleanUp() = 0;
  virtual void addActor(GameActorPtr actor) = 0;
  virtual void removeActor(uint id) = 0;
  virtual int getNumActors() = 0;
  virtual uint getNextActorID() = 0;
  virtual bool gotActor(uint id) = 0;
  virtual void addInputHandler(InputHandlerPtr handler) = 0;
  virtual void removeInputHandler(InputHandlerPtr handler) = 0;
  virtual long getDeltaTime() = 0;
  virtual void startFrame() = 0;
  virtual void processInputStage() = 0;
  virtual void processUpdateStage() = 0;
  virtual void processDrawStage() = 0;
  virtual void endFrame(float maxFPS) = 0;
  virtual Camera* getCamera() = 0;
  virtual InputManager* getInputManager() = 0;
  virtual RenderContext* getRenderContext() = 0;
  };


