
#include <UISystem/UIPanel.h>
#include <UISystem/UIButton.h>
#include <UISystem/UIList.h>
#include "TOMainMenuContext.h"
#include "Resources.h"
#include "SettingsDlg.h"


TOMainMenuContext::TOMainMenuContext(const RenderContextPtr& renderContext, std::shared_ptr<TOSettings> settings)
  : GameContextImpl(renderContext), settings(settings)
  {}

typedef std::pair<string, OnMouseClickCallback> MenuOption;

bool TOMainMenuContext::initialise()
  {
  bool result = GameContextImpl::initialise();

  std::vector<MenuOption> menuButtons;

  //  NEW
  menuButtons.emplace_back(IMAGE_MAINMENU_NEW, [this](uint mouseX, uint mouseY)
    {
    currentOutcome.optionSelected = optionNew;
    endContext();
    return true;
    });

  //  LOAD
  menuButtons.emplace_back(IMAGE_MAINMENU_LOAD, [this](uint mouseX, uint mouseY)
    {
    currentOutcome.optionSelected = optionLoad;
    showLoadDlg();
    return true;
    });

  //  SETTINGS
  menuButtons.emplace_back(IMAGE_MAINMENU_SETTINGS, [this](uint mouseX, uint mouseY)
    {
    currentOutcome.optionSelected = optionSettings;
    showSettingsDlg();
    return true;
    });

  //  QUIT
  menuButtons.emplace_back(IMAGE_MAINMENU_QUIT, [this](uint mouseX, uint mouseY)
    {
    currentOutcome.optionSelected = optionQuit;
    endContext();
    return true;
    });

  //  load all the buttons
  const Vector2D buttonSize(200, 100);
  const Vector3D colour(0.3, 0.3, 0.3);
  const Vector3D pressColour(0.2, 0.3, 0.5);
  const float buttonPadding = 50;
  float buttonYOffset = 200;
  for (MenuOption& menuOption : menuButtons)
    {
    UIButton* button = new UIButton(getUIManager()->getNextComponentID(), false);
    button->setButtonTexture(getRenderContext()->getSharedTexture(menuOption.first));
    button->setButtonHighlightColour(pressColour, colour);
    button->setSize(buttonSize);
    button->setHorizontalAlignment(Alignment::alignmentCentre);
    button->setOffset(Vector2D(0, buttonYOffset));
    button->setMouseClickCallback(menuOption.second);
    getUIManager()->addComponent(UIComponentPtr(button));
    buttonYOffset += buttonPadding + buttonSize.y;
    }
  return result;
  }

FontPtr TOMainMenuContext::getDefaultFont()
  {
  return getRenderContext()->getSharedFont(FONT_DEFAULT_FNT, FONT_DEFAULT_GLYPHS);
  }

TOMainMenuContext::MainMenuOutcome TOMainMenuContext::doMainMenu(RenderContextPtr renderContext, std::shared_ptr<TOSettings> settings)
  {
  mathernogl::logInfo("Loading main menu...");
  TOMainMenuContext mainMenuContext(renderContext, settings);
  mainMenuContext.initialise();
  while(!mainMenuContext.isContextEnded() && renderContext->isWindowOpen())
    {
    mainMenuContext.startFrame();
    mainMenuContext.processInputStage();
    mainMenuContext.processUpdateStage();
    mainMenuContext.processDrawStage();
    mainMenuContext.endFrame(30);
    }
  TOMainMenuContext::MainMenuOutcome outcome = mainMenuContext.getSelectedOption();
  mainMenuContext.cleanUp();
  renderContext->reset();
  return outcome;
  }

void TOMainMenuContext::showLoadDlg()
  {
  std::shared_ptr<SaveLoadDlg> saveLoadDlg(new SaveLoadDlg(getUIManager()->getNextComponentID(), SaveLoadDlg::modeLoad));
  getUIManager()->addComponent(saveLoadDlg);
  getUIManager()->pushModalComponent(saveLoadDlg);

  //  On loaded from a file
  saveLoadDlg->setLoadGameStateCallback([this](std::shared_ptr<TOGameState> state)
      {
      currentOutcome.optionSelected = optionLoad;
      currentOutcome.loadedState = state;
      endContext();
      });

  //  On cancelled loading
  uint id = saveLoadDlg->getID();
  saveLoadDlg->setCancelledCallback([this, id]()
      {
      getUIManager()->removeComponent(id);
                                      getUIManager()->popModalComponent();
      });
  }

void TOMainMenuContext::showSettingsDlg()
  {
  std::shared_ptr<SettingsDlg> settingsDlg(new SettingsDlg(getUIManager()->getNextComponentID()));
  getUIManager()->addComponent(settingsDlg);
  getUIManager()->pushModalComponent(settingsDlg);
  uint id = settingsDlg->getID();
  settingsDlg->setOnFinishedCallback([this, id]()
     {
     getUIManager()->removeComponent(id);
     getUIManager()->popModalComponent();
     });
  }