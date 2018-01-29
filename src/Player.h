#pragma once

#include <GameSystem/GameSystem.h>
#include "Tower.h"

/*
*   
*/



class Player : public GameActor
  {
protected:
  const uint playerNum;
  TowerList ownedTowers;

public:
  Player(uint actorID, uint playerNum);

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

  TowerList* getOwnedTowersList() { return &ownedTowers; }
  };

typedef std::shared_ptr<Player> PlayerPtr;
