#pragma once

#include <src/RenderSystem/RenderableLines.h>
#include "Tower.h"

/*
*   
*/

struct ConnectionNode;
typedef std::shared_ptr<ConnectionNode> ConnectionNodePtr;

struct ConnectionNode
  {
  TowerPtr tower;
  mathernogl::MappedList<ConnectionNodePtr> connections;
  int networkID = -1;
  };

struct Network
  {
  uint id = 0;
  std::list<ConnectionNodePtr> powerSrcNodes;
  };
typedef std::shared_ptr<Network> NetworkPtr;

class ConnectionManager : public GameActor
  {
private:
  mathernogl::MappedList<ConnectionNodePtr> nodes;
  mathernogl::MappedList<NetworkPtr> networks;
  std::shared_ptr<RenderableLines> beamsRenderable;
  std::map<std::pair<uint, uint>, uint> connectionBeamIDs;
  std::list<std::pair<uint, uint>> beamsToCreate;
  std::list<std::pair<uint, uint>> beamsToRemove;

public:
  ConnectionManager(uint id);

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

  void addTower(TowerPtr tower);
  void removeTower(uint id);
  bool isTowerConnectedToPowerSrc(uint id) const;
  void rebuildNetworks();
  bool containsTower(uint id) const;
  bool areTowersConnectable(TowerPtr towerA, TowerPtr towerB) const;

protected:
  void addConnection(ConnectionNodePtr nodeA, ConnectionNodePtr nodeB);
  void removeConnection(ConnectionNodePtr nodeA, ConnectionNodePtr nodeB);
  void addPendingRenderables(RenderContext* renderContext);
  void removePendingRenderables(RenderContext* renderContext);

  };
