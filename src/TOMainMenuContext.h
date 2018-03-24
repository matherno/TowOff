#pragma once

#include <GameSystem/GameContextImpl.h>

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

private:
  Option selectedOption = optionNone;

public:
  TOMainMenuContext(const RenderContextPtr& renderContext);
  Option getSelectedOption() { return selectedOption; }

  virtual bool initialise() override;
  };
