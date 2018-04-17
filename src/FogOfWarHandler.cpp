//
// Created by matt on 14/04/18.
//

#include "FogOfWarHandler.h"
#include "TOGameContext.h"

FogOfWarHandler::FogOfWarHandler(uint id, uint visibilityTextureWidth) : GameActor(id), visibilityTextureWidth(visibilityTextureWidth)
  {}

void FogOfWarHandler::onAttached(GameContext* gameContext)
  {
  mathernogl::clearGLErrors();

  TextureOptions textureOptions;
  textureOptions.filtering = mathernogl::LINEAR;
  textureOptions.wrapping = mathernogl::CLAMP_BORDER;
  visibilityTexture = gameContext->getRenderContext()->createEmptyTexture(visibilityTextureWidth, visibilityTextureWidth, 1, textureOptions);
  const float mapWidth = TOGameContext::cast(gameContext)->getMapWidth();

  RenderContext* renderContext = gameContext->getRenderContext();
  fogRenderable.reset(new FogOfWarRenderable(renderContext->getNextRenderableID(), 175));
  fogRenderable->initialise(renderContext);
  fogRenderable->setWorldVisibilityTexture(visibilityTexture, mapWidth, mapWidth);
  renderContext->getRenderableSet()->addRenderable(fogRenderable);

  ASSERT_NO_GL_ERROR();
  }

void FogOfWarHandler::onUpdate(GameContext* gameContext)
  {
  fogRenderable->setTransform(mathernogl::matrixTranslate(TOGameContext::cast(gameContext)->getCameraFocalPosition()));
  }

void FogOfWarHandler::onDetached(GameContext* gameContext)
  {
  visibilityTexture->cleanUp();
  visibilityTexture.reset();

  RenderContext* renderContext = gameContext->getRenderContext();
  fogRenderable->cleanUp(renderContext);
  renderContext->getRenderableSet()->removeRenderable(fogRenderable->getID());
  }

void FogOfWarHandler::refreshFOW(GameContext* gameContext)
  {
  if (visibilityTexture)
    {
    TOGameContext* toGameContext = TOGameContext::cast(gameContext);
    const uint width = visibilityTexture->width;
    const uint height = visibilityTexture->height;
    const uint bpp = visibilityTexture->bytesPerPixel;
    for (int row = 0; row < height; ++row)
      {
      for (int col = 0; col < width; ++col)
        {
        Vector3D worldPos((float)col / width, 0, (float)row / height);
        worldPos *= toGameContext->getMapWidth();
        worldPos -= toGameContext->getMapWidth() * 0.5;
        worldPos.z *= -1;
        worldPos.y = 0;
        float visibility = getVisibilityAt(gameContext, worldPos, fadeDistance);
        visibilityTexture->bytes.get()[bpp * (col + row * width)] = mathernogl::byte(255.0 * visibility);
        }
      }
    mathernogl::updateTexture(visibilityTexture.get());
    }
  }

/*static*/ float FogOfWarHandler::getVisibilityAt(GameContext* gameContext, const Vector3D& worldPos, float fadeDistance)
  {
  TOGameContext* toGameContext = TOGameContext::cast(gameContext);
  float visibility = 0;
  for (const TowerPtr& tower : *toGameContext->getTowers()->getList())
    {
    if (tower->isUnderConstruction())
      continue;

    const float fadeStart = tower->getVisibilityRadius() - fadeDistance;
    const float towerVisibility = ((float)worldPos.distanceToPoint(tower->getPosition()) - fadeStart) / fadeDistance;
    visibility += mathernogl::clampf(1 - towerVisibility, 0, 1);
    }
  return mathernogl::clampf(visibility, 0, 1);
  }