//
// Created by matt on 28/01/18.
//

#include "HUDHandler.h"
#include "TowerFactory.h"
#include "TOGameContext.h"
#include "Resources.h"

/*
 *  TowerFocusPanel
 */

void TowerFocusPanel::initialise(GameContext* context)
  {
  UIPanel::initialise(context);

  UIManager* uiManager = context->getUIManager();
  icon.reset(new UIPanel(uiManager->getNextComponentID()));
  icon->setOffset(Vector2D(20, 10));
  icon->setSize(Vector2D(75));
  icon->setColour(Vector3D(0.2));
  addChild(icon);

  static const Vector2D healthBarOffset = Vector2D(120, 15);
  static const Vector2D statsBarSize = Vector2D(200, 23);
  static const uint statsBarsGap = 10;
  static const uint statsTextGap = 5;
  static const uint statsTextSize = 18;

  healthBar.reset(new UIProgressBar(uiManager->getNextComponentID()));
  healthBar->setOffset(healthBarOffset);
  healthBar->setSize(statsBarSize);
  healthBar->setColour(Vector3D(0.2));
  healthBar->setBarColour(Vector3D(0.6, 0.1, 0.1));
  addChild(healthBar);

  healthText.reset(new UIText(uiManager->getNextComponentID()));
  healthText->setOffset(healthBarOffset + Vector2D(statsBarSize.x + statsTextGap, 0));
  healthText->setSize(statsBarSize);
  healthText->setFontSize(statsTextSize);
  healthText->setFontColour(Vector3D(0));
  healthText->showBackground(false);
  healthText->setTextCentreAligned(true, false);
  addChild(healthText);

  energyBar.reset(new UIProgressBar(uiManager->getNextComponentID()));
  energyBar->setOffset(healthBarOffset + Vector2D(0, statsBarSize.y + statsBarsGap));
  energyBar->setSize(statsBarSize);
  energyBar->setColour(Vector3D(0.2));
  energyBar->setBarColour(Vector3D(0, 0.2, 0.8));
  addChild(energyBar);

  energyText.reset(new UIText(uiManager->getNextComponentID()));
  energyText->setOffset(healthBarOffset + Vector2D(statsBarSize.x + statsTextGap, statsBarSize.y + statsBarsGap));
  energyText->setSize(statsBarSize);
  energyText->setFontSize(statsTextSize);
  energyText->setFontColour(Vector3D(0));
  energyText->showBackground(false);
  energyText->setTextCentreAligned(true, false);
  addChild(energyText);

  nameText.reset(new UIText(uiManager->getNextComponentID()));
  nameText->setOffset(Vector2D(20, -5));
  nameText->setSize(Vector2D(250, 40));
  nameText->setVerticalAlignment(alignmentEnd);
  nameText->setFontSize(30);
  nameText->setFontColour(Vector3D(0));
  nameText->showBackground(false);
  addChild(nameText);
  }

string createProgressString(int amount, int max)
  {
  std::ostringstream stringStream;
  stringStream << amount << "/" << max;
  return stringStream.str();
  }

void TowerFocusPanel::updateTowerInfo(GameContext* context)
  {
  TOGameContext* toGameContext = TOGameContext::cast(context);

  if (TowerPtr focusedTower = toGameContext->getFocusedTower())
    {
    setVisible(true, true);

    bool newFocusTower = false;
    if (!activeFocusTower || focusedTower->getID() != activeFocusTower->getID())
      {
      activeFocusTower = focusedTower;
      const TowerType* type = TowerFactory::getTowerType(activeFocusTower->getTowerType());
      icon->setTexture(context->getRenderContext()->getSharedTexture(type->iconFilePath));
      icon->invalidate();
      nameText->setText(type->name);
      nameText->invalidate();
      newFocusTower = true;
      }

    const int healthPoints = focusedTower->getHealthPoints();
    const int maxHealthPoints = focusedTower->getMaxHealthPoints();
    if (UIProgressBar::updateProgressBar(healthBar.get(), healthPoints, maxHealthPoints) || newFocusTower)
      {
      healthText->setText(createProgressString(healthPoints, maxHealthPoints));
      healthText->invalidate();
      }

    const int storedEnergy = focusedTower->getStoredEnergy();
    const int maxEnergy = focusedTower->getMaxEnergy();
    if(UIProgressBar::updateProgressBar(energyBar.get(), storedEnergy, maxEnergy) || newFocusTower)
      {
      energyText->setText(createProgressString(storedEnergy, maxEnergy));
      energyText->invalidate();
      }
    }
  else
    {
    setVisible(false, true);
    activeFocusTower.reset();
    }
  }


/*
 *  HUDHandler
 */

void HUDHandler::initialiseUI(GameContext* context)
  {
  UIManager* uiManager = context->getUIManager();
  mainUIPanel.reset(new UIPanel(uiManager->getNextComponentID()));
  mainUIPanel->setOffset(Vector2D(0, 0));
  mainUIPanel->setSize(Vector2D(300, 170));
  mainUIPanel->setColour(Vector3D(0.4, 0.3, 0.1));
  mainUIPanel->setHorizontalAlignment(alignmentEnd);
  mainUIPanel->setVerticalAlignment(alignmentEnd);
  mainUIPanel->setWidthMatchParent(true);
  uiManager->addComponent(mainUIPanel);

  setupTowerBuildPanel(context);
  setupTowerFocusPanel(context);
  }

void HUDHandler::updateUI(GameContext* context)
  {
  towerFocusPanel->updateTowerInfo(context);
  }

void HUDHandler::deselectTowerType()
  {
  towerButtonGroup->forceDeselectAll();
  }

void HUDHandler::setupTowerFocusPanel(GameContext* context)
  {
  UIManager* uiManager = context->getUIManager();
  towerFocusPanel.reset(new TowerFocusPanel(uiManager->getNextComponentID()));
  towerFocusPanel->setOffset(Vector2D(-10, 0));
  towerFocusPanel->setSize(Vector2D(600, 100));
  towerFocusPanel->setColour(Vector3D(0.3, 0.3, 0.25));
  towerFocusPanel->setVerticalAlignment(alignmentCentre);
  towerFocusPanel->setHorizontalAlignment(alignmentEnd);
  towerFocusPanel->setHeightMatchParent(true);
  towerFocusPanel->setPadding(10, 10);
  mainUIPanel->addChild(towerFocusPanel);
  }

void HUDHandler::setupTowerBuildPanel(GameContext* context)
  {
  UIManager* uiManager = context->getUIManager();

  UIPanel* subPanel = new UIPanel(uiManager->getNextComponentID());
  subPanel->setOffset(Vector2D(0, 0));
  subPanel->setSize(Vector2D(100, 100));
  subPanel->setColour(Vector3D(0.35, 0.35, 0.3));
  subPanel->setVerticalAlignment(alignmentCentre);
  subPanel->setHorizontalAlignment(alignmentCentre);
  subPanel->setHeightMatchParent(true);
  subPanel->setWidthMatchParent(true);
  subPanel->setPadding(5, 5);
  mainUIPanel->addChild(UIComponentPtr(subPanel));

  towerButtonGroup.reset(new UIToggleButtonGroup());

  int towerNum = 0;
  const std::map<uint, TowerType>* towerTypes = TowerFactory::getTowerTypeMap();
  for (const std::pair<uint, TowerType> pair : *towerTypes)
    {
    const uint towerTypeID = pair.first;
    const TowerType& towerType = pair.second;

    UIButton* button = new UIButton(uiManager->getNextComponentID(), true);
    button->setOffset(Vector2D(50 + towerNum * 70, 20));
    button->setSize(Vector2D(50, 50));
    button->setButtonTexture(context->getRenderContext()->getSharedTexture(towerType.iconFilePath));
    button->setButtonHighlightColour(Vector3D(0.3, 0.3, 1));
    button->setVerticalAlignment(alignmentStart);
    button->setHorizontalAlignment(alignmentStart);
    button->setHighlightWidth(3);
    button->setGroup(towerButtonGroup);
    button->setMouseClickCallback([this, towerTypeID, button, context](uint x, uint y) -> bool
                                    {
                                    if (button->isToggledDown())
                                      startTowerPlacingMode(context, towerTypeID);
                                    else
                                      endTowerPlacingMode(context);
                                    return true;
                                    });
    subPanel->addChild(UIComponentPtr(button));
    ++towerNum;
    }
  }

void HUDHandler::endTowerPlacingMode(GameContext* gameContext)
  {
  if (placementHandler)
    {
    gameContext->removeInputHandler(placementHandler);
    placementHandler.reset();
    }
  deselectTowerType();
  }

void HUDHandler::startTowerPlacingMode(GameContext* gameContext, uint towerTypeID)
  {
  TOGameContext::cast(gameContext)->getSelectionManager()->deselectAll(gameContext);

  if (placementHandler)
    {
    gameContext->removeInputHandler(placementHandler);
    placementHandler.reset();
    }
  placementHandler.reset(new TowerPlacementHandler(gameContext->getInputManager()->getNextHandlerID(), towerTypeID));
  placementHandler->setEndHandlerCallback([this, gameContext]() { endTowerPlacingMode(gameContext); });
  gameContext->addInputHandler(placementHandler);
  }

void HUDHandler::cleanUp(GameContext* context)
  {
  endTowerPlacingMode(context);
  }

