#pragma once

#include <GameSystem/GameContextImpl.h>
#include "SaveLoadDlg.h"

/*
*   
*/

class TOMainMenuContext : public GameContextImpl
  {
public:
  enum Option
    {
    optionNone,
    optionNew,
    optionQuit,
    optionLoad,
    };

  struct MainMenuOutcome
    {
    Option optionSelected = optionNone;
    std::shared_ptr<TOGameState> loadedState;
    bool isNewGame() const { return optionSelected == optionNew; }
    bool isLoadGame() const { return optionSelected == optionLoad; }
    bool isQuitGame() const { return optionSelected == optionQuit; }
    };

private:
  MainMenuOutcome currentOutcome;

public:
  TOMainMenuContext(const RenderContextPtr& renderContext);
  MainMenuOutcome getSelectedOption() { return currentOutcome; }
  virtual FontPtr getDefaultFont() override;
  virtual bool initialise() override;
  static MainMenuOutcome doMainMenu(RenderContextPtr renderContext);

protected:
  void showLoadDlg();
  };
