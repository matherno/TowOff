#pragma once

#include <GameSystem/GameSystem.h>
#include <RenderSystem/RenderableMesh.h>

/*
*   
*/

class Deposit : public GameActor
  {
private:
  std::shared_ptr<RenderableMesh> modelRenderable;
  Vector3D position;
  float hitRadius = 2.4;
  uint storedEnergy = 0;

public:
  Deposit(uint id);
  void setPosition(const Vector3D& position);
  Vector3D getPosition() const { return position; }
  float getHitRadius() const { return hitRadius; }
  void setStoredEnergyAmount(uint energy) { this->storedEnergy = energy; }
  uint getStoredEnergyAmount() const { return storedEnergy; }
  uint takeEnergy(uint amount, bool allOrNothing);    // returns the amount taken

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

protected:
  void randomiseTransform();
  };

typedef std::shared_ptr<Deposit> DepositPtr;
typedef mathernogl::MappedList<DepositPtr> DepositList;