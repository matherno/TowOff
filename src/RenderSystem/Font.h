#pragma once

#include <mathernogl/MathernoGL.h>

/*
*   
*/

struct Font
  {
  const mathernogl::FontDefinitionPtr fontDefinition;
  std::shared_ptr<mathernogl::Texture> fontGlyphsPage;

  Font(mathernogl::FontDefinitionPtr& fontDefinition, std::shared_ptr<mathernogl::Texture>& fontGlyphsPage) : fontDefinition(fontDefinition), fontGlyphsPage(fontGlyphsPage) {}
  };

typedef std::shared_ptr<Font> FontPtr;
