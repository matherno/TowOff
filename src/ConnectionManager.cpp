//
// Created by matt on 1/02/18.
//

#include <src/RenderSystem/RenderableMesh.h>
#include "ConnectionManager.h"
#include "Resources.h"
#include "TimeToLiveActor.h"


ConnectionManager::ConnectionManager(uint id) : GameActor(id)
  {}

void ConnectionManager::onAttached(GameContext* gameContext)
  {
  beamsRenderable.reset(new RenderableLines(gameContext->getRenderContext()->getNextRenderableID()));
  beamsRenderable->initialise(gameContext->getRenderContext());
  gameContext->getRenderContext()->getRenderableSet()->addRenderable(beamsRenderable);
  }

void ConnectionManager::onUpdate(GameContext* gameContext)
  {
  addPendingRenderables(gameContext->getRenderContext());
  removePendingRenderables(gameContext->getRenderContext());
  }

void ConnectionManager::onDetached(GameContext* gameContext)
  {
  beamsRenderable->cleanUp(gameContext->getRenderContext());
  gameContext->getRenderContext()->getRenderableSet()->removeRenderable(beamsRenderable->getID());
  }

void ConnectionManager::addTower(TowerPtr tower)
  {
  if (containsTower(tower->getID()))
    return;

  ConnectionNodePtr newNode(new ConnectionNode());
  newNode->tower = tower;
  for (ConnectionNodePtr& node : *nodes.getList())
    {
    if (areTowersConnectable(node->tower, tower))
      addConnection(node, newNode);
    }

  nodes.add(newNode, tower->getID());
  rebuildNetworks();
  }

void ConnectionManager::removeTower(uint id)
  {
  if (!containsTower(id))
    return;

  ConnectionNodePtr oldNode = nodes.get(id);
  nodes.remove(id);
  for (ConnectionNodePtr& node : *nodes.getList())
    removeConnection(node, oldNode);
  rebuildNetworks();
  }

void recurseNodeConnections(ConnectionNodePtr& parentNode, Network* network)
  {
  for (ConnectionNodePtr& node : *parentNode->connections.getList())
    {
    if (node->networkID == -1)
      {
      if (node->tower->isPowerSrc())
        network->powerSrcNodes.push_back(node);
      node->networkID = network->id;
      recurseNodeConnections(node, network);
      }
    }
  }

void ConnectionManager::rebuildNetworks()
  {
  networks.clear();
  for (ConnectionNodePtr& node : *nodes.getList())
    node->networkID = -1;

  uint nextNetworkID = 0;
  for (ConnectionNodePtr& node : *nodes.getList())
    {
    if (node->networkID == -1)
      {
      uint networkID = nextNetworkID++;
      NetworkPtr thisNetwork(new Network());
      thisNetwork->id = networkID;

      node->networkID = networkID;
      if (node->tower->isPowerSrc())
        thisNetwork->powerSrcNodes.push_back(node);
      recurseNodeConnections(node, thisNetwork.get());
      networks.add(thisNetwork, networkID);
      }
    }
  }

bool ConnectionManager::containsTower(uint id) const
  {
  return nodes.contains(id);
  }

bool ConnectionManager::areTowersConnectable(TowerPtr towerA, TowerPtr towerB)
  {
  if (!areTowersInRange(towerA->getPosition(), towerA->getConnectRadius(), towerB->getPosition(), towerB->getConnectRadius()))
    return false;
  return areTowerFunctionsCompatible(towerA->getFunction(), towerB->getFunction());
  }

bool ConnectionManager::areTowersInRange(const Vector3D& towerAPos, float towerAConnectRadius, const Vector3D& towerBPos, float towerBConnectRadius)
  {
  float minRadius = std::min(towerAConnectRadius, towerBConnectRadius);
  return towerAPos.distanceToPoint(towerBPos) <= minRadius;
  }

bool ConnectionManager::areTowerFunctionsCompatible(Tower::TowerFunction towerAFunction, Tower::TowerFunction towerBFunction)
  {
  if (towerAFunction == Tower::combat || towerBFunction == Tower::combat)
    return false;
  if (towerAFunction == Tower::relay || towerBFunction == Tower::relay)
    return true;
  return towerAFunction != towerBFunction;
  }

void ConnectionManager::addConnection(ConnectionNodePtr nodeA, ConnectionNodePtr nodeB)
  {
  nodeA->connections.add(nodeB, nodeB->tower->getID());
  nodeB->connections.add(nodeA, nodeA->tower->getID());
  beamsToCreate.emplace_back(nodeA->tower->getID(), nodeB->tower->getID());
  }

void ConnectionManager::removeConnection(ConnectionNodePtr nodeA, ConnectionNodePtr nodeB)
  {
  nodeA->connections.remove(nodeB->tower->getID());
  nodeB->connections.remove(nodeA->tower->getID());
  beamsToRemove.emplace_back(nodeA->tower->getID(), nodeB->tower->getID());
  }

void ConnectionManager::addPendingRenderables(RenderContext* renderContext)
  {
  for (std::pair<uint, uint>& nodePair : beamsToCreate)
    {
    if (connectionBeamIDs.count(nodePair) > 0)
      continue;

    TowerPtr firstTower = nodes.get(nodePair.first)->tower;
    TowerPtr secondTower = nodes.get(nodePair.second)->tower;
    Vector3D start = firstTower->getConnectPosition();
    Vector3D end = secondTower->getConnectPosition();
    Vector3D colour(0.2, 0.2, 0.7);
    if (firstTower->getFunction() == Tower::relay && secondTower->getFunction() == Tower::relay)
      colour.set(0, 0.1, 0.6);
    connectionBeamIDs[nodePair] = beamsRenderable->addLine(start, end, colour, 0.8);
    }
  beamsToCreate.clear();
  }

void ConnectionManager::removePendingRenderables(RenderContext* renderContext)
  {
  for (std::pair<uint, uint>& nodePair : beamsToRemove)
    {
    if (connectionBeamIDs.count(nodePair) > 0)
      {
      uint lineID = connectionBeamIDs[nodePair];
      beamsRenderable->removeLine(lineID);
      }
    else
      {
      std::pair<uint, uint> nodePairReverse = std::make_pair(nodePair.second, nodePair.first);
      if (connectionBeamIDs.count(nodePairReverse) > 0)
        {
        uint lineID = connectionBeamIDs[nodePairReverse];
        beamsRenderable->removeLine(lineID);
        }
      }
    }
  beamsToRemove.clear();
  }

const Network* ConnectionManager::getTowersNetwork(uint towerID) const
  {
  if (!containsTower(towerID))
    return nullptr;

  int networkID = nodes.get(towerID)->networkID;
  if (networkID < 0 || !networks.contains((uint)networkID))
    return nullptr;

  return networks.get((uint)networkID).get();
  }

bool ConnectionManager::areTowersConnected(uint towerID1, uint towerID2) const
  {
  const Network* towerNetwork1 = getTowersNetwork(towerID1);
  const Network* towerNetwork2 = getTowersNetwork(towerID2);
  if (towerNetwork1 && towerNetwork2)
    return towerNetwork1->id == towerNetwork2->id;
  return false;
  }
