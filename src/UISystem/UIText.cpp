//
// Created by matt on 27/02/18.
//

#include <set>
#include "UIText.h"

const static std::set<uint> whitespaceCharacters =
  {
    (uint)'\n',
    (uint)'\t',
    (uint)'\r',
    (uint)'\v',
    (uint)' ',
  };


UIText::UIText(uint id, FontPtr font) : UIPanel(id), font(font)
  {
  setColour(Vector3D(1));
  }

void UIText::initialise(GameContext* context)
  {
  if (!font)
    font = context->getDefaultFont();
  UIPanel::initialise(context);
  setVisible(true, false);
  }

void UIText::setText(string text)
  {
  this->text = text;
  }

void UIText::refresh(GameContext* context, const Vector2D& parentPos, const Vector2D& parentSize)
  {
  UIPanel::refresh(context, parentPos, parentSize);

  if (font)
    {
    for (UIComponentPtr comp : characterComponents)
      removeChild(comp->getID());
    characterComponents.clear();

    // cursor pos is aligned with the top of such text line
    Vector2D cursor = Vector2D(0, 0);
    float textScaling = (float)textSize / (float)font->fontDefinition->getLineHeight();
    buildText(context, &cursor, textScaling);
    }
  }

void UIText::buildCharacter(GameContext* context, FontCharPtr fontCharacter, Vector2D* cursor, float textScaling)
  {
  UIPanel* component = new UIPanel(context->getUIManager()->getNextComponentID());
  component->setHorizontalAlignment(alignmentStart);
  component->setVerticalAlignment(alignmentStart);
  component->setOffset(*cursor + fontCharacter->offset * textScaling);
  component->setSize(fontCharacter->size * textScaling);
  component->setColour(fontColour);
  component->setTexture(font->fontGlyphsPage, true);
  component->setTextureCoords(fontCharacter->texCoordBL, fontCharacter->texCoordTR);
  cursor->x += fontCharacter->cursorAdvance * textScaling;

  UIComponentPtr componentPtr(component);
  characterComponents.push_back(componentPtr);
  addChild(componentPtr);
  }

void UIText::buildWord(GameContext* context, const FontWord& fontWord, Vector2D* cursor, float textScaling)
  {
  for (FontCharPtr fontChar : fontWord)
    buildCharacter(context, fontChar, cursor, textScaling);
  }

void UIText::buildText(GameContext* context, Vector2D* cursor, float textScaling)
  {
  const Vector2D textBounds = getCurrentScreenSize();
  const uint lineHeight = (uint)(font->fontDefinition->getLineHeight() * textScaling);

  uint spaceAdvance = 10;
  if (spaceWidth >= 0)
    spaceAdvance = (uint)spaceWidth;
  else if (font->fontDefinition->containsCharacter(' '))
    spaceAdvance = (uint)font->fontDefinition->getCharacterDefinition(' ')->cursorAdvance;
  spaceAdvance *= textScaling;

  FontWord fontWord;
  uint currentWordLength = 0;
  uint numLines = 1;
  uint maxLineLength = 0;
  for (const char& character : text)
    {
    uint ascii = (uint)character;
    if (isValidNonWhiteSpaceCharacter(ascii))
      {
      FontCharPtr fontChar = font->fontDefinition->getCharacterDefinition(ascii);
      fontWord.push_back(fontChar);

      currentWordLength += fontChar->cursorAdvance * textScaling;
      if (cursor->x + currentWordLength > textBounds.x && cursor->x != 0)
        {
        maxLineLength = (uint)std::max((float)maxLineLength, cursor->x);
        cursor->x = 0;
        cursor->y += lineHeight;
        ++numLines;
        }
      continue;
      }

    if (cursor->y + lineHeight <= textBounds.y)
      {
      buildWord(context, fontWord, cursor, textScaling);
      fontWord.clear();
      currentWordLength = 0;
      }
    else
      {
      fontWord.clear();
      break;
      }

    if (character == '\n')
      {
      maxLineLength = (uint)std::max((float)maxLineLength, cursor->x);
      cursor->x = 0;
      cursor->y += lineHeight;
      ++numLines;
      }
    else if (character == ' ')
      {
      cursor->x += spaceAdvance;
      }
    }

  if (!fontWord.empty())
    buildWord(context, fontWord, cursor, textScaling);
  maxLineLength = (uint)std::max((float)maxLineLength, cursor->x);

  if (centreAlignVert)
    {
    const uint paragraphHeight = numLines * lineHeight;
    const uint textShift = (uint)((textBounds.y - paragraphHeight) * 0.5f);
    for (UIComponentPtr comp : characterComponents)
      comp->setOffset(comp->getOffset() + Vector2D(0, textShift));
    }

  if (centreAlignHoriz)
    {
    //  not the proper way of doing it, should be aligning line by line (works for single line text)
    const uint textShift = (uint)((textBounds.x - maxLineLength) * 0.5f);
    for (UIComponentPtr comp : characterComponents)
      comp->setOffset(comp->getOffset() + Vector2D(textShift, 0));
    }
  }

bool UIText::isValidNonWhiteSpaceCharacter(uint ascii) const
  {
  if (whitespaceCharacters.find(ascii) != whitespaceCharacters.end())
    return false;

  return font->fontDefinition->containsCharacter(ascii);
  }

void UIText::setVisible(bool visible, bool recurseChildren)
  {
  UIPanel::setVisible(visible, recurseChildren);
  if (visible)
    UIPanel::setVisible(background, false);
  }
