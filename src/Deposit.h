#pragma once

#include <GameSystem/GameSystem.h>
#include <RenderSystem/RenderableMesh.h>
#include <ParticleSystem/ParticleSystem.h>
#include <GameSystem/Timer.h>

/*
*   
*/

class Deposit : public GameActor
  {
private:
  std::shared_ptr<RenderableMesh> modelRenderable;
  std::shared_ptr<ParticleSystem> ambientParticles;
  TexturePtr modelTexture;
  Vector3D position;
  float hitRadius = 2.4;
  uint storedEnergy = 0;
  uint maxEnergy = 1000;
  uint energyRegenPerSec = 3;   //  energy per second
  Timer updateTimer;

public:
  Deposit(uint id);
  void setPosition(const Vector3D& position);
  Vector3D getPosition() const { return position; }
  float getHitRadius() const { return hitRadius; }
  void setTotalEnergy(uint energy) { this->storedEnergy = energy; this->maxEnergy = energy; }
  uint getStoredEnergyAmount() const { return storedEnergy; }
  uint takeEnergy(uint amount, bool allOrNothing);    // returns the amount taken
  void setEnergyRegenRate(uint energyPerSecond) { energyRegenPerSec = energyPerSecond; }

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

protected:
  void randomiseTransform();
  };

typedef std::shared_ptr<Deposit> DepositPtr;
typedef mathernogl::MappedList<DepositPtr> DepositList;