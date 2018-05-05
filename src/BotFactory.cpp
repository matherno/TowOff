//
// Created by matt on 29/04/18.
//

#include "BotFactory.h"
#include "Resources.h"
#include "Bot.h"

#define BOT_STAR 0

const std::map<uint, BotType> botTypes =
  {
    //{   id,     BotType{     base mesh file,  hit radius,   initial health}},
    {BOT_STAR,    BotType{      MESH_BOT_STAR,      0.8,              10 }},
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
    return BotPtr(bot);
    }

  return nullptr;
  }

BotPortalPtr BotFactory::createBotPortal(uint id, const Vector3D& position)
  {
  static std::shared_ptr<BotPortalSpawnDef> basicSpawnDef(new BotPortalSpawnDef(5000, {0, 0, 0, 0, 0}));

  BotPortal* portal = new BotPortal(id, MESH_BOTPORTAL);
  portal->setPosition(position);
  portal->setHitRadius(3);
  portal->setMaxHealthPoints(200);
  portal->setHealthPoints(200);
  portal->setPortalSpawnDef(basicSpawnDef);
  return BotPortalPtr(portal);
  }
