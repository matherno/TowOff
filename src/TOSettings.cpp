//
// Created by matt on 19/04/18.
//

#include "TOSettings.h"

#define CONFIG_FILE "toconfig.ini"

TOSettings::TOSettings()
  {
  manager.reset(new GameConfigManager());
  idxScreenWidth = manager->addOption(GameConfigManager::typeInt, "Screen Width", "screenx", "graphics", "1366");
  idxScreenHeight = manager->addOption(GameConfigManager::typeInt, "Screen Height", "screeny", "graphics", "768");
  idxAntiAliasing = manager->addOption(GameConfigManager::typeBool, "Anti-Aliasing", "antialiasing", "graphics", "true");
  idxCamPanSpeed = manager->addOption(GameConfigManager::typeFloat, "Camera Pan Speed Factor", "campanspeed", "input", "1");
  idxCamRotSpeed = manager->addOption(GameConfigManager::typeFloat, "Camera Rotate Speed Factor", "camrotspeed", "input", "1");
  idxFullscreen = manager->addOption(GameConfigManager::typeBool, "Fullscreen", "fullscreen", "graphics", "false");
  idxEnableEdgePan = manager->addOption(GameConfigManager::typeBool, "Enable Screen Edge Pan", "screenedgepan", "input", "true");
  }

void TOSettings::initialise()
  {
  activeValueMap = createValueMap();
  if(std::ifstream(CONFIG_FILE))
    readSettingsFile(activeValueMap.get());
  else
    writeSettingsFile(activeValueMap.get());
  }

std::shared_ptr<GameConfigValueMap> TOSettings::createValueMap()
  {
  std::shared_ptr<GameConfigValueMap> map = std::make_shared<GameConfigValueMap>(manager);
  map->setDefaultValues();
  return map;
  }

void TOSettings::setActiveValueMap(std::shared_ptr<GameConfigValueMap> map)
  {
  activeValueMap = map;
  }

const GameConfigManager* TOSettings::getConfigManager() const
  {
  return manager.get();
  }

void TOSettings::readSettingsFile(GameConfigValueMap* map) const
  {
  map->readIniFile(CONFIG_FILE);
  }

void TOSettings::writeSettingsFile(const GameConfigValueMap* map) const
  {
  map->writeIniFile(CONFIG_FILE);
  }

uint TOSettings::getWindowWidth() const
  {
  int width = 800;
  activeValueMap->getIntValue(idxScreenWidth, &width);
  return (uint)width;
  }

uint TOSettings::getWindowHeight() const
  {
  int width = 800;
  activeValueMap->getIntValue(idxScreenHeight, &width);
  return (uint)width;
  }

bool TOSettings::useAntiAliasing() const
  {
  bool aa = true;
  activeValueMap->getBoolValue(idxAntiAliasing, &aa);
  return aa;
  }

bool TOSettings::useFullscreen() const
  {
  bool fullscreen = false;
  activeValueMap->getBoolValue(idxFullscreen, &fullscreen);
  return fullscreen;
  }

float TOSettings::getCameraPanSpeed() const
  {
  float speed = 1;
  activeValueMap->getFloatValue(idxCamPanSpeed, &speed);
  return speed;
  }

float TOSettings::getCameraRotSpeed() const
  {
  float speed = 1;
  activeValueMap->getFloatValue(idxCamRotSpeed, &speed);
  return speed;
  }

bool TOSettings::enableScreenEdgePan() const
  {
  bool enable = false;
  activeValueMap->getBoolValue(idxEnableEdgePan, &enable);
  return enable;
  }


