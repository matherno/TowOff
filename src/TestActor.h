#pragma once


#include <GameSystem/GameSystem.h>

class TestActor : public GameActor
  {
private:
  RenderablePtr tree;

public:
  TestActor(uint id);

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;
  void translate(Vector3D translation);
  };
