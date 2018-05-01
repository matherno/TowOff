//
// Created by matt on 29/04/18.
//

#include "BotFactory.h"
#include "Resources.h"
#include "Bot.h"

#define BOT_SLUG 0

const std::map<uint, BotType> botTypes =
  {
    //{   id,     BotType{     base mesh file,  hit radius,   initial health}},
    {BOT_SLUG,    BotType{      MESH_BOT_SLUG,      0.8,              30 }},
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
