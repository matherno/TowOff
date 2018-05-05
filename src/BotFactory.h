#pragma once

#include <Bot.h>
#include "BotPortal.h"

/*
*   
*/

struct BotType
  {
  string meshFilePath;
  float hitRadius;
  uint initHealth;
  };

class BotFactory
  {
public:
  static BotPtr createBot(uint botType, uint id, const Vector3D& position);
  static BotPortalPtr createBotPortal(uint id, const Vector3D& position);
  };
