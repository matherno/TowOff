#pragma once

#include <GameSystem/GameSystem.h>
#include "BotPortal.h"
#include "Bot.h"
#include "BotQTNode.h"

struct BotType
  {
  string name;
  string meshFilePath;
  float hitRadius;
  uint initHealth;
  float speed;
  uint countLimit;
  };

class BotFactory
  {
public:
  static BotPtr createBot(uint botType, uint id, const Vector3D& position);
  static BotPortalPtr createBotPortal(uint id, const Vector3D& position);
  static const std::map<uint, BotType>* getBotTypes();
  };


class BotManager : public GameActor
  {
private:
  BotList botList;
  BotPortalList botPortalList;
  std::shared_ptr<BotQTNode> botQuadTreeRoot;
  std::map<uint, std::shared_ptr<RenderableMesh>> botRenderables;
  std::map<uint, MeshStorageInstancedPtr> botMeshStorages;
  std::map<uint, uint> botTypeCounts;

public:
  BotManager(uint id);

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

  BotPtr createBot(GameContext* gameContext, uint botType, const Vector3D& position);
  void removeBot(GameContext* gameContext, uint id);
  BotPortalPtr createBotPortal(GameContext* gameContext, const Vector3D& position);
  void removeBotPortal(GameContext* gameContext, uint id);

  const BotList* getBotList() const { return &botList; }
  const BotPortalList* getBotPortalList() const { return &botPortalList; }
  const BotQTNode* getBotQuadTreeRoot();
  bool canCreateBot(uint botType) const;
  uint getBotTypeCount(uint botType) const;

protected:
  void constructBotQuadTree();
  void incrementBotCount(uint botType);
  void decrementBotCount(uint botType);
  };
