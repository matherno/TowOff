#pragma once

#include <RenderSystem/RenderableMesh.h>
#include <GameSystem/Timer.h>
#include "TowerTarget.h"

/*
*   
*/

struct BotPortalSpawnDef
  {
  std::vector<uint> spawnsIDs;      // defines all the bot IDs to spawn, every [timeBetweenSpawns] ms
  long timeBetweenSpawns = 10000;
  BotPortalSpawnDef(long timeBetween, const std::vector<uint>& spawns) : timeBetweenSpawns(timeBetween), spawnsIDs(spawns) {};
  };

class BotPortal : public TowerTarget
  {
private:
  const string meshFilePath;
  std::shared_ptr<RenderableMesh> renderable;
  std::shared_ptr<BotPortalSpawnDef> spawnDef;
  Timer spawnTimer;
  int spawnIndex = -1;

public:
  BotPortal(uint id, const string& meshFilePath);

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

  void setPortalSpawnDef(std::shared_ptr<BotPortalSpawnDef> spawnDef);

protected:
  void destroyThis(GameContext* gameContext);
  void doSpawning(GameContext* gameContext);
  };

typedef std::shared_ptr<BotPortal> BotPortalPtr;
typedef mathernogl::MappedList<BotPortalPtr> BotPortalList;