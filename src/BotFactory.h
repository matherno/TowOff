#pragma once

#include <Bot.h>

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
  };
