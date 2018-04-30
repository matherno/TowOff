#pragma once

#include <RenderSystem/RenderableMesh.h>
#include "TowerTarget.h"

/*
*   
*/

class Bot : public TowerTarget
  {
private:
  const uint typeID;
  const string meshFilePath;
  std::shared_ptr<RenderableMesh> renderable;
  std::unique_ptr<Vector3D> targetPosition;
  float movementSpeed = 0.5;

public:
  Bot(uint id, uint botType, const string& meshFilePath);
  void moveToPosition(const Vector3D& position);
  void setMovementSpeed(float speed) { movementSpeed = speed; }

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

protected:
  void doMovement(GameContext* gameContext);
  };

typedef std::shared_ptr<Bot> BotPtr;
typedef mathernogl::MappedList<BotPtr> BotList;