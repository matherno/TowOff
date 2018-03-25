
#include <UISystem/UIPanel.h>
#include <UISystem/UIButton.h>
#include "TOMainMenuContext.h"
#include "Resources.h"


TOMainMenuContext::TOMainMenuContext(const RenderContextPtr& renderContext) : GameContextImpl(renderContext)
  {}

typedef std::pair<string, OnMouseClickCallback> MenuOption;

bool TOMainMenuContext::initialise()
  {
  bool result = GameContextImpl::initialise();

  std::vector<MenuOption> menuButtons;

  //  NEW
  menuButtons.emplace_back(IMAGE_MAINMENU_NEW, [this](uint mouseX, uint mouseY)
    {
    selectedOption = optionNew;
    endContext();
    return true;
    });

  //  LOAD
  menuButtons.emplace_back(IMAGE_MAINMENU_LOAD, [this](uint mouseX, uint mouseY)
    {
//    selectedOption = optionQuit;
//    endContext();
    return true;
    });

  //  QUIT
  menuButtons.emplace_back(IMAGE_MAINMENU_QUIT, [this](uint mouseX, uint mouseY)
    {
    selectedOption = optionQuit;
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
    UIButton* buttonNew = new UIButton(getUIManager()->getNextComponentID(), true);
    buttonNew->setButtonTexture(getRenderContext()->getSharedTexture(menuOption.first));
    buttonNew->setButtonHighlightColour(pressColour, colour);
    buttonNew->setSize(buttonSize);
    buttonNew->setHorizontalAlignment(Alignment::alignmentCentre);
    buttonNew->setOffset(Vector2D(0, buttonYOffset));
    buttonNew->setMouseClickCallback(menuOption.second);
    getUIManager()->addComponent(UIComponentPtr(buttonNew));
    buttonYOffset += buttonPadding + buttonSize.y;
    }
  return result;
  }
