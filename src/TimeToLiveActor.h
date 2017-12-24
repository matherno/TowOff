#pragma once

#include <src/GameSystem/GameSystem.h>

/*
*   Actor that removes itself after the given number of milliseconds
*   Will cleanup and remove its stored renderables when it does
*/

class TimeToLiveActor : public GameActor
  {
protected:
  long timeToLive = 2000;
  std::list<RenderablePtr> renderables;
  bool firstUpdate = true;

public:
  TimeToLiveActor(uint id, long timeToLive = 200);
  void setTimeToLive(long timeToLive) { this->timeToLive = timeToLive; }
  void addRenderable(RenderablePtr renderable){ renderables.push_back(renderable); }
  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;
  };
