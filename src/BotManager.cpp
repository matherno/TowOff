//
// Created by matt on 4/06/18.
//

#include "BotManager.h"
#include "Resources.h"
#include "Bot.h"

#define BOT_STAR 0
#define BOT_COMET 1

const std::map<uint, BotType> botTypes =
  {
    //{   id,     BotType{   name,       base mesh file,  hit radius,   initial health,   speed,   count limit}},
    {BOT_STAR,    BotType{ "Star Bot",    MESH_BOT_STAR,      0.8,              10,         5,          300}},
    {BOT_COMET,   BotType{ "Comet Bot",   MESH_BOT_COMET,     1.0,              30,         4,          200}},
  };

const std::map<uint, BotType>* BotFactory::getBotTypes()
  {
  return &botTypes;
  }

BotPtr BotFactory::createBot(uint botType, uint id, const Vector3D& position)
  {
  if (botTypes.count(botType) > 0)
    {
    const BotType& type = botTypes.at(botType);
    Bot* bot = new Bot(id, botType);
    bot->setPosition(position);
    bot->setHitRadius(type.hitRadius);
    bot->setMaxHealthPoints(type.initHealth);
    bot->setHealthPoints(type.initHealth);
    bot->setMovementSpeed(type.speed);
    bot->setCentreOfMassOffset(Vector3D(0, 1, 0));
    return BotPtr(bot);
    }

  return nullptr;
  }

BotPortalPtr BotFactory::createBotPortal(uint id, const Vector3D& position)
  {
  static std::shared_ptr<BotPortalSpawnDef> basicSpawnDef(new BotPortalSpawnDef(5000, {BOT_COMET, BOT_COMET, BOT_STAR, BOT_STAR, BOT_STAR}));

  BotPortal* portal = new BotPortal(id, MESH_BOTPORTAL);
  portal->setPosition(position);
  portal->setHitRadius(3);
  portal->setMaxHealthPoints(200);
  portal->setHealthPoints(200);
  portal->setPortalSpawnDef(basicSpawnDef);
  return BotPortalPtr(portal);
  }


BotManager::BotManager(uint id) : GameActor(id)
  {}

void BotManager::onAttached(GameContext* gameContext)
  {
  RenderContext* renderContext = gameContext->getRenderContext();

  for (auto botType : botTypes)
    {
    std::shared_ptr<RenderableMesh> renderable = std::shared_ptr<RenderableMesh>(new RenderableMesh(renderContext->getNextRenderableID()));
    MeshStorageInstancedPtr meshStorage = renderContext->createInstancedMeshStorage(botType.second.meshFilePath, botType.second.countLimit);
    renderable->setMeshStorage(meshStorage);
    renderable->setDrawStyleTexture(renderContext->getSharedTexture(IMAGE_TEXTURE_PALETTE));
    renderable->initialise(renderContext);
    renderContext->getRenderableSet()->addRenderable(renderable);
    botRenderables[botType.first] = renderable;
    botMeshStorages[botType.first] = meshStorage;
    }
  }

void BotManager::onUpdate(GameContext* gameContext)
  {
  botQuadTreeRoot.reset();

  for (auto botType : botTypes)
    botMeshStorages[botType.first]->clearInstances();

  for (BotPtr bot : *botList.getList())
    botMeshStorages[bot->getType()]->addInstance(bot->getTransform()->getTransformMatrix());

  for (auto botType : botTypes)
    botMeshStorages[botType.first]->updateInstanceBuffer();
  }

void BotManager::onDetached(GameContext* gameContext)
  {
  for (auto botType : botTypes)
    {
    botRenderables[botType.first]->cleanUp(gameContext->getRenderContext());
    gameContext->getRenderContext()->getRenderableSet()->removeRenderable(botRenderables[botType.first]->getID());
    botRenderables[botType.first].reset();
    botMeshStorages[botType.first]->cleanUp();
    }
  }

const BotQTNode* BotManager::getBotQuadTreeRoot()
  {
  if (!botQuadTreeRoot)
    constructBotQuadTree();
  return botQuadTreeRoot.get();
  }

void BotManager::constructBotQuadTree()
  {
  botQuadTreeRoot = BotQTNode::constructBotQuadTree(10, 5, 10, &botList);
  }

BotPtr BotManager::createBot(GameContext* gameContext, uint botType, const Vector3D& position)
  {
  BotPtr bot = BotFactory::createBot(botType, gameContext->getNextActorID(), position);
  if (!bot)
    return nullptr;

  botList.add(bot, bot->getID());
  gameContext->addActor(bot);
  incrementBotCount(botType);
  return bot;
  }

void BotManager::removeBot(GameContext* gameContext, uint id)
  {
  if (botList.contains(id))
    {
    BotPtr bot = botList.get(id);
    decrementBotCount(bot->getType());
    botList.remove(id);
    gameContext->removeActor(id);
    }
  }

BotPortalPtr BotManager::createBotPortal(GameContext* gameContext, const Vector3D& position)
  {
  BotPortalPtr portal = BotFactory::createBotPortal(gameContext->getNextActorID(), position);
  if (!portal)
    return nullptr;

  botPortalList.add(portal, portal->getID());
  gameContext->addActor(portal);
  return portal;
  }

void BotManager::removeBotPortal(GameContext* gameContext, uint id)
  {
  if (botPortalList.contains(id))
    {
    botPortalList.remove(id);
    gameContext->removeActor(id);
    }
  }

bool BotManager::canCreateBot(uint botType) const
  {
  if (botTypes.count(botType) > 0)
    return getBotTypeCount(botType) < botTypes.at(botType).countLimit;
  return false;
  }

uint BotManager::getBotTypeCount(uint botType) const
  {
  if (botTypeCounts.count(botType) > 0)
    return botTypeCounts.at(botType);
  return 0;
  }

void BotManager::incrementBotCount(uint botType)
  {
  if (botTypeCounts.count(botType) > 0)
    ++botTypeCounts[botType];
  else
    botTypeCounts[botType] = 1;
  }

void BotManager::decrementBotCount(uint botType)
  {
  if (botTypeCounts.count(botType) > 0)
    --botTypeCounts[botType];
  }
