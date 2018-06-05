//
// Created by matt on 3/06/18.
//

#include "BotQTNode.h"

#define CHILD_TOP_LEFT      0
#define CHILD_TOP_RIGHT     1
#define CHILD_BOTTOM_LEFT   2
#define CHILD_BOTTOM_RIGHT  3

static const std::list<uint> childIdxs = { CHILD_TOP_LEFT, CHILD_TOP_RIGHT, CHILD_BOTTOM_LEFT, CHILD_BOTTOM_RIGHT };

BotQTNode::BotQTNode(const Vector2D& min, const Vector2D& max) : min(min), max(max)
  {
  }

void BotQTNode::initialise(uint treeDepth, uint maxDepth, uint maxBotsInLeaf, float minBoundsWidth, const std::vector<BotPtr>* bots)
  {
  depth = treeDepth;
  const Vector2D halfBounds = (max - min) * 0.5f;
  bool canSplit = true;
  canSplit = canSplit && treeDepth < maxDepth;
  canSplit = canSplit && bots->size() > maxBotsInLeaf;
  canSplit = canSplit && halfBounds.x > minBoundsWidth && halfBounds.y > minBoundsWidth;
  if (!canSplit)
    {
    // don't split
    nodeBots.insert(nodeBots.begin(), bots->begin(), bots->end());
    isLeaf = true;
    }
  else
    {
    //  create children
    const Vector2D mid = (max + min) * 0.5f;
    children[CHILD_TOP_LEFT].reset(new BotQTNode(Vector2D(min.x, mid.y), Vector2D(mid.x, max.y)));
    children[CHILD_TOP_RIGHT].reset(new BotQTNode(Vector2D(mid.x, mid.y), Vector2D(max.x, max.y)));
    children[CHILD_BOTTOM_LEFT].reset(new BotQTNode(Vector2D(min.x, min.y), Vector2D(mid.x, mid.y)));
    children[CHILD_BOTTOM_RIGHT].reset(new BotQTNode(Vector2D(mid.x, min.y), Vector2D(max.x, mid.y)));

    //  split bot indices into children
    std::array<std::vector<BotPtr>, 4> childBotIndices;
    for (BotPtr bot : *bots)
      {
      Vector2D pos = bot->getPosition2D();
      for (uint childIdx : childIdxs)
        {
        if (children[childIdx]->isPointWithin(pos))
          {
          childBotIndices[childIdx].push_back(bot);
          break;
          }
        }
      }

    //  process the children
    for (uint childIdx : childIdxs)
      children[childIdx]->initialise(treeDepth + 1, maxDepth, maxBotsInLeaf, minBoundsWidth, &childBotIndices[childIdx]);
    isLeaf = false;
    }
  }

bool BotQTNode::isPointWithin(const Vector2D& point)
  {
  return point.x >= min.x && point.x <= max.x && point.y >= min.y && point.y <= max.y;
  }

void BotQTNode::findBotsWithinRange(const Vector3D& rangeCentre, float rangeRadius, const BotList* botList, std::vector<BotPtr>* foundBots) const
  {
  findBotsWithinRange(Vector2D(rangeCentre.x, rangeCentre.z), rangeRadius, botList, foundBots);
  }

void BotQTNode::findBotsWithinRange(const Vector2D& rangeCentre, float rangeRadius, const BotList* botList, std::vector<BotPtr>* foundBots) const
  {
  //  don't bother if this is an empty leaf node
  if (isLeafNode() && nodeBots.size() < 0)
    return;

  //  as long as the range is within this nodes bounds...
  if (!mathernogl::doesRectCircleIntersect(min, max, rangeCentre, rangeRadius))
    return;

  if (isLeafNode())
    {
    //  check bots in this leaf node to find those in range
    for (BotPtr bot : nodeBots)
      {
      if (bot->isInRange(rangeCentre, rangeRadius, 0))
        foundBots->push_back(bot);
      }
    }
  else
    {
    //  check child nodes
    for (const std::unique_ptr<BotQTNode>& child : children)
      child->findBotsWithinRange(rangeCentre, rangeRadius, botList, foundBots);
    }
  }

void BotQTNode::outputNodeToLog(uint treeDepth) const
  {
  string indent;
  for (uint lvl = 2; lvl <= treeDepth; ++lvl)
    indent += " ";
  string msg;
  msg += indent + "Min: (" + std::to_string(min.x) + " " + std::to_string(min.y) + "), Max: (" + std::to_string(max.x) + " " + std::to_string(max.y) + ")";
  if (nodeBots.size() > 0)
    msg += " Bots: " + std::to_string(nodeBots.size());
  mathernogl::logInfo(msg);
  if (isLeafNode())
    {
    }
  else
    {
    for (const std::unique_ptr<BotQTNode>& child : children)
      child->outputNodeToLog(treeDepth + 1);
    }
  }

uint BotQTNode::getMaxTreeDepth() const
  {
  if (isLeafNode())
    {
    return depth;
    }
  else
    {
    uint maxDepth = 0;
    for (const std::unique_ptr<BotQTNode>& child : children)
      maxDepth = std::max(child->getMaxTreeDepth(), maxDepth);
    return maxDepth;
    }
  }

uint BotQTNode::getNodeCount() const
  {
  if (isLeafNode())
    {
    return 1;
    }
  else
    {
    uint count = 1;
    for (const std::unique_ptr<BotQTNode>& child : children)
      count += child->getNodeCount();
    return count;
    }
  }

uint BotQTNode::getLeafCount() const
  {
  if (isLeafNode())
    {
    return 1;
    }
  else
    {
    uint count = 0;
    for (const std::unique_ptr<BotQTNode>& child : children)
      count += child->getLeafCount();
    return count;
    }
  }

/*static*/ std::shared_ptr<BotQTNode> BotQTNode::constructBotQuadTree(uint maxDepth, uint maxBotsInLeaf, float minBoundsWidth, const BotList* botList)
  {
  if (botList->count() == 0)
    return nullptr;

  std::vector<BotPtr> botsList;
  Vector2D min, max;
  bool first = true;
  for (BotPtr bot : *botList->getList())
    {
    Vector2D position = bot->getPosition2D();
    if (first)
      {
      min = position;
      max = position;
      first = false;
      }
    else
      {
      min.x = std::min(min.x, position.x);
      min.y = std::min(min.y, position.y);
      max.x = std::max(max.x, position.x);
      max.y = std::max(max.y, position.y);
      }
    botsList.push_back(bot);
    }

  std::shared_ptr<BotQTNode> root(new BotQTNode(min, max));
  root->initialise(1, maxDepth, maxBotsInLeaf, minBoundsWidth, &botsList);
  return root;
  }



