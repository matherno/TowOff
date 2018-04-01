#pragma once

#include "UIText.h"

/*
*   Just supports single line text properly at the moment
*/

class UIEditText : public UIText
  {
private:
  uint characterLimit;
  bool onlyAllowAlphaNumeric = false;

public:
  UIEditText(uint id, const FontPtr& font = nullptr);
  virtual bool keyPress(GameContext* context, uint key) override;
  virtual void onGainFocus(GameContext* context) override;
  virtual void onLossFocus(GameContext* context) override;
  void setCharacterLimit(uint limit) { characterLimit = limit; }
  void setOnlyAllowAlphaNumeric(bool alphaNumeric) { onlyAllowAlphaNumeric = alphaNumeric; }

protected:
  void moveCaretLeft(int num = 1);
  void moveCaretRight(int num = 1);
  void moveCaret(int num);
  void insertAtCaret(const string& text);
  void backspaceAtCaret();
  void deleteAtCaret();
  void moveCaretToStart();
  void moveCaretToEnd();
  };
