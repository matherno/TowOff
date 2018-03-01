#pragma once

#include "UIPanel.h"

/*
*   
*/

class UIText : public UIPanel
  {
private:
  std::vector<UIComponentPtr> characterComponents;
  FontPtr font;
  string text;
  uint textSize = 25;
  Vector3D fontColour;
  int spaceWidth = -1;   // -1 => use fonts space width definition
  bool background = true;
  bool centreAlign = false;

public:
  UIText(uint id, FontPtr font = nullptr);      // will use default font if one not given
  void setText(string text);
  void setFontSize(uint textSize) { this->textSize = std::max(textSize, 1u); }
  void setFontColour (const Vector3D& colour) { fontColour = colour; }
  void setSpaceWidth (uint width) { spaceWidth = width; }
  void setTextCentreAligned(bool align) { centreAlign = align; }
  void showBackground(bool show) { background = show; }

  virtual void initialise(GameContext* context) override;
  virtual void refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize) override;
  virtual void setVisible(bool visible, bool recurseChildren) override;

protected:
  typedef const mathernogl::FontCharacter* FontCharPtr;
  typedef std::vector<FontCharPtr> FontWord;
  void buildCharacter(GameContext* context, FontCharPtr fontCharacter, Vector2D* cursor, float textScaling);
  void buildWord(GameContext* context, const FontWord& fontWord, Vector2D* cursor, float textScaling);
  void buildText(GameContext* context, Vector2D* cursor, float textScaling);
  bool isValidNonWhiteSpaceCharacter(uint ascii) const;
  };
