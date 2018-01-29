//
// Created by matt on 28/01/18.
//

#include <src/UISystem/UIButton.h>
#include "HUDHandler.h"
#include "Resources.h"
#include "TowerFactory.h"
#include "TOGameContext.h"

void HUDHandler::initialiseUI(GameContext* context)
  {
//  TOGameContext* toContext = TOGameContext::cast(context);

  UIManager* uiManager = context->getUIManager();
  mainUIPanel.reset(new UIPanel(uiManager->getNextComponentID()));
  mainUIPanel->setOffset(Vector2D(0, 0));
  mainUIPanel->setSize(Vector2D(300, 170));
  mainUIPanel->setBackgroundColour(Vector3D(0.4, 0.3, 0.1));
  mainUIPanel->setHorizontalAlignment(alignmentEnd);
  mainUIPanel->setVerticalAlignment(alignmentEnd);
//  panel->setHeightMatchParent(true);
  mainUIPanel->setWidthMatchParent(true);
  uiManager->addComponent(mainUIPanel);

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

  UIToggleButtonGroupPtr toggleButtonGroup(new UIToggleButtonGroup());

  const std::vector<TowerType>* towerTypes = TowerFactory::getTowerTypeList();
  for (uint towerNum = 0; towerNum < towerTypes->size(); ++towerNum){
    const TowerType& towerType = (*towerTypes)[towerNum];
    UIButton* button = new UIButton(uiManager->getNextComponentID(), true);
    button->setOffset(Vector2D(50 + towerNum * 70, 20));
    button->setSize(Vector2D(50, 50));
    button->setButtonTexture(context->getRenderContext()->createTexture(towerType.iconFilePath));
    button->setButtonHighlightColour(Vector3D(0.3, 0.3, 1));
    button->setVerticalAlignment(alignmentStart);
    button->setHorizontalAlignment(alignmentStart);
    button->setHighlightWidth(3);
    button->setGroup(toggleButtonGroup);
    button->setMouseClickCallback([this, towerNum, button](uint x, uint y) -> bool
                                    {
                                    if (button->isToggledDown())
                                      onSelectedTowerType(towerNum);
                                    else
                                      onDeselectedTowerType();
                                    return true;
                                    });
    subPanel->addChild(UIComponentPtr(button));
  }
  }

void HUDHandler::onSelectedTowerType(uint towerType)
  {
  selectedTowerType = towerType;
  }

void HUDHandler::onDeselectedTowerType()
  {
  selectedTowerType = -1;
  }
