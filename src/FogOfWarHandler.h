#pragma once

#include <GameSystem/GameSystem.h>
#include "FogOfWarRenderable.h"

/*
*   
*/

class FogOfWarHandler : public GameActor
  {
private:
  const uint visibilityTextureWidth;
  TexturePtr visibilityTexture;
  std::shared_ptr<FogOfWarRenderable> fogRenderable;
  float fadeDistance = 5;

public:
  FogOfWarHandler(uint id, uint visibilityTextureWidth);
  void refreshFOW(GameContext* gameContext);
  void setState(bool isOn);

  virtual void onAttached(GameContext* gameContext) override;
  virtual void onUpdate(GameContext* gameContext) override;
  virtual void onDetached(GameContext* gameContext) override;

  //  fadeDistance => the distance that visibility fades from 0 to 1, at the end of a towers visibility radius
  static float getVisibilityAtPosition(GameContext* gameContext, const Vector3D& worldPos, float fadeDistance);
  static bool isVisibleAtPosition(GameContext* gameContext, const Vector3D& worldPos);
  };
