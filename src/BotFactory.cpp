//
// Created by matt on 29/04/18.
//

#include "BotFactory.h"
#include "Resources.h"
#include "Bot.h"

#define BOT_STAR 0
#define BOT_COMET 1

const std::map<uint, BotType> botTypes =
  {
    //{   id,     BotType{     base mesh file,  hit radius,   initial health,   speed}},
    {BOT_STAR,    BotType{      MESH_BOT_STAR,      0.8,              10,         5 }},
    {BOT_COMET,   BotType{      MESH_BOT_COMET,     1.0,              30,         4 }},
  };

BotPtr BotFactory::createBot(uint botType, uint id, const Vector3D& position)
  {
  if (botTypes.count(botType) > 0)
    {
    const BotType& type = botTypes.at(botType);
    Bot* bot = new Bot(id, botType, type.meshFilePath);
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
