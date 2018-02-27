//
// Created by matt on 28/01/18.
//

#include "HUDHandler.h"
#include "TowerFactory.h"
#include "TOGameContext.h"
#include "Resources.h"

void HUDHandler::initialiseUI(GameContext* context)
  {
  UIManager* uiManager = context->getUIManager();
  mainUIPanel.reset(new UIPanel(uiManager->getNextComponentID()));
  mainUIPanel->setOffset(Vector2D(0, 0));
  mainUIPanel->setSize(Vector2D(300, 170));
  mainUIPanel->setBackgroundColour(Vector3D(0.4, 0.3, 0.1));
  mainUIPanel->setHorizontalAlignment(alignmentEnd);
  mainUIPanel->setVerticalAlignment(alignmentEnd);
  mainUIPanel->setWidthMatchParent(true);
  uiManager->addComponent(mainUIPanel);

  setupTowerBuildPanel(context);
  setupTowerFocusPanel(context);
  }

void HUDHandler::updateUI(GameContext* context)
  {
  TOGameContext* toGameContext = TOGameContext::cast(context);
  if (TowerPtr focusedTower = toGameContext->getFocusedTower())
    {
    if (!activeHUDTower || activeHUDTower->getID() != focusedTower->getID())
      {
      const TowerType* type = TowerFactory::getTowerType(focusedTower->getTowerType());
      activeHUDTower = focusedTower;
      towerFocusPanel->setVisible(true, true);
      towerIcon->setBackgroundTexture(context->getRenderContext()->createTexture(type->iconFilePath));
      towerIcon->invalidate();
      }

    //  update health bar
    UIProgressBar::updateProgressBar(towerHealthBar.get(), focusedTower->getHealthPoints(), focusedTower->getMaxHealthPoints());

    //  update energy bar
    UIProgressBar::updateProgressBar(towerEnergyBar.get(), focusedTower->getStoredEnergy(), focusedTower->getMaxEnergy());
    }
  else
    {
    towerFocusPanel->setVisible(false, true);
    activeHUDTower.reset();
    }
  }

void HUDHandler::deselectTowerType()
  {
  towerButtonGroup->forceDeselectAll();
  }

void HUDHandler::setupTowerFocusPanel(GameContext* context)
  {
  UIManager* uiManager = context->getUIManager();

  towerFocusPanel.reset(new UIPanel(uiManager->getNextComponentID()));
  towerFocusPanel->setOffset(Vector2D(-10, 0));
  towerFocusPanel->setSize(Vector2D(600, 100));
  towerFocusPanel->setBackgroundColour(Vector3D(0.3, 0.3, 0.25));
  towerFocusPanel->setVerticalAlignment(alignmentCentre);
  towerFocusPanel->setHorizontalAlignment(alignmentEnd);
  towerFocusPanel->setHeightMatchParent(true);
  towerFocusPanel->setPadding(10, 10);
  mainUIPanel->addChild(towerFocusPanel);

  towerIcon.reset(new UIPanel(uiManager->getNextComponentID()));
  towerIcon->setOffset(Vector2D(20, 10));
  towerIcon->setSize(Vector2D(75));
  towerIcon->setBackgroundColour(Vector3D(0.2));
  towerIcon->setVerticalAlignment(alignmentStart);
  towerIcon->setHorizontalAlignment(alignmentStart);
  towerFocusPanel->addChild(towerIcon);

  towerEnergyBar.reset(new UIProgressBar(uiManager->getNextComponentID()));
  towerEnergyBar->setOffset(Vector2D(120, 45));
  towerEnergyBar->setSize(Vector2D(200, 20));
  towerEnergyBar->setBackgroundColour(Vector3D(0.2));
  towerEnergyBar->setVerticalAlignment(alignmentStart);
  towerEnergyBar->setHorizontalAlignment(alignmentStart);
  towerEnergyBar->setBarColour(Vector3D(0, 0.2, 0.8));
  towerFocusPanel->addChild(towerEnergyBar);

  towerHealthBar.reset(new UIProgressBar(uiManager->getNextComponentID()));
  towerHealthBar->setOffset(Vector2D(120, 15));
  towerHealthBar->setSize(Vector2D(200, 20));
  towerHealthBar->setBackgroundColour(Vector3D(0.2));
  towerHealthBar->setVerticalAlignment(alignmentStart);
  towerHealthBar->setHorizontalAlignment(alignmentStart);
  towerHealthBar->setBarColour(Vector3D(0.6, 0.1, 0.1));
  towerFocusPanel->addChild(towerHealthBar);
  }

void HUDHandler::setupTowerBuildPanel(GameContext* context)
  {
  UIManager* uiManager = context->getUIManager();

  UIPanel* subPanel = new UIPanel(uiManager->getNextComponentID());
  subPanel->setOffset(Vector2D(0, 0));
  subPanel->setSize(Vector2D(100, 100));
  subPanel->setBackgroundColour(Vector3D(0.35, 0.35, 0.3));
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
    button->setButtonTexture(context->getRenderContext()->createTexture(towerType.iconFilePath));
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
  TOGameContext::cast(gameContext)->unfocusTower();

  if (placementHandler)
    {
    gameContext->removeInputHandler(placementHandler);
    placementHandler.reset();
    }
  placementHandler.reset(new TowerPlacementHandler(gameContext->getInputManager()->getNextHandlerID(), towerTypeID));
  placementHandler->setEndHandlerCallback([this, gameContext]() { endTowerPlacingMode(gameContext); });
  gameContext->addInputHandler(placementHandler);
  }

