//
// Created by matt on 1/02/18.
//

#include <RenderSystem/RenderableMesh.h>
#include "ConnectionManager.h"
#include "Resources.h"
#include "TimeToLiveActor.h"
#include "UnderConstructTower.h"
#include "TowerFactory.h"


ConnectionManager::ConnectionManager(uint id) : GameActor(id)
  {}

void ConnectionManager::onAttached(GameContext* gameContext)
  {
  beamsRenderable.reset(new RenderableLines(gameContext->getRenderContext()->getNextRenderableID(), DRAW_STAGE_TRANSPARENT));
  beamsRenderable->initialise(gameContext->getRenderContext());
  gameContext->getRenderContext()->getRenderableSet()->addRenderable(beamsRenderable);
  }

void ConnectionManager::onUpdate(GameContext* gameContext)
  {
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
  if (towerA->isUnderConstruction() && towerB->isUnderConstruction())
    return false;
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
  if (towerAFunction == Tower::none || towerBFunction == Tower::none)
    return false;
  if (towerAFunction == Tower::combat || towerBFunction == Tower::combat)
    return false;
  if (TowerFactory::canTowerRelayEnergy(towerAFunction) || TowerFactory::canTowerRelayEnergy(towerBFunction))
    return true;
  return towerAFunction != towerBFunction;
  }

void ConnectionManager::addConnection(ConnectionNodePtr nodeA, ConnectionNodePtr nodeB)
  {
  uint towerAID = nodeA->tower->getID();
  uint towerBID = nodeB->tower->getID();
  nodeA->connections.add(nodeB, towerBID);
  nodeB->connections.add(nodeA, towerAID);

  TowerPtr firstTower = nodeA->tower;
  TowerPtr secondTower = nodeB->tower;
  Vector3D start = firstTower->getConnectPosition();
  Vector3D end = secondTower->getConnectPosition();
  Vector3D colour(0.2, 0.2, 0.7);
  if (firstTower->getFunction() == Tower::relay && secondTower->getFunction() == Tower::relay)
    colour.set(0, 0.1, 0.6);
  if (firstTower->isUnderConstruction() || secondTower->isUnderConstruction())
    colour.set(0.4, 0.4, 0.1);

  colour.x = pow(colour.x, 2.2);
  colour.y = pow(colour.y, 2.2);
  colour.z = pow(colour.z, 2.2);

  connectionBeamIDs[std::make_pair(towerAID, towerBID)] = beamsRenderable->addLine(start, end, colour, 0.8);
  }

void ConnectionManager::removeConnection(ConnectionNodePtr nodeA, ConnectionNodePtr nodeB)
  {
  uint towerAID = nodeA->tower->getID();
  uint towerBID = nodeB->tower->getID();
  nodeA->connections.remove(towerBID);
  nodeB->connections.remove(towerAID);

  std::pair<uint, uint> nodePair(towerAID, towerBID);
  if (connectionBeamIDs.count(nodePair) > 0)
    {
    uint lineID = connectionBeamIDs[nodePair];
    beamsRenderable->removeLine(lineID);
    }
  else
    {
    nodePair = std::make_pair(towerBID, towerAID);
    if (connectionBeamIDs.count(nodePair) > 0)
      {
      uint lineID = connectionBeamIDs[nodePair];
      beamsRenderable->removeLine(lineID);
      }
    }
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
