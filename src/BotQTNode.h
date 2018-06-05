#pragma once

#include <list>
#include <GameSystem/GameSystem.h>
#include <Bot.h>

/*
*   
*/

class BotQTNode
  {
private:
  std::vector<BotPtr> nodeBots;
  std::array<std::unique_ptr<BotQTNode>, 4> children;
  Vector2D min;
  Vector2D max;
  bool isLeaf = false;
  uint depth = 0;

public:
  BotQTNode(const Vector2D& min, const Vector2D& max);
  void initialise(uint treeDepth, uint maxDepth, uint maxBotsInLeaf, float minBoundsWidth, const std::vector<BotPtr>* bots);
  bool isLeafNode() const { return isLeaf; }
  Vector2D getMin() const { return min; }
  Vector2D getMax() const { return max; }
  bool isPointWithin(const Vector2D& point);
  void outputNodeToLog(uint treeDepth = 1) const;
  uint getMaxTreeDepth() const;
  uint getNodeCount() const;
  uint getLeafCount() const;

  void findBotsWithinRange(const Vector3D& rangeCentre, float rangeRadius, const BotList* botList, std::vector<BotPtr>* foundBots) const;
  void findBotsWithinRange(const Vector2D& rangeCentre, float rangeRadius, const BotList* botList, std::vector<BotPtr>* foundBots) const;

  static std::shared_ptr<BotQTNode> constructBotQuadTree(uint maxDepth, uint maxBotsInLeaf, float minBoundsWidth, const BotList* botList);
  };
