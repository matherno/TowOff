//
// Created by matt on 5/05/18.
//

#include "BotPortal.h"
#include "Resources.h"
#include "TOGameContext.h"

BotPortal::BotPortal(uint id, const string& meshFilePath) : TowerTarget(id), meshFilePath(meshFilePath)
  {}

void BotPortal::onAttached(GameContext* gameContext)
  {
  TowerTarget::onAttached(gameContext);

  RenderContext* renderContext = gameContext->getRenderContext();
  renderable.reset(new RenderableMesh(renderContext->getNextRenderableID()));
  renderable->setMeshStorage(renderContext->getSharedMeshStorage(meshFilePath));
  renderable->setDrawStyleTexture(renderContext->getSharedTexture(IMAGE_TEXTURE_PALETTE));
  renderable->getTransform()->setTransformMatrix(mathernogl::matrixTranslate(getPosition()));
  renderable->initialise(renderContext);
  renderContext->getRenderableSet()->addRenderable(renderable);
  }

void BotPortal::onUpdate(GameContext* gameContext)
  {
  if (!isAlive())
    {
    destroyThis(gameContext);
    return;
    }

  doSpawning(gameContext);
  TowerTarget::onUpdate(gameContext);
  }

void BotPortal::onDetached(GameContext* gameContext)
  {
  TowerTarget::onDetached(gameContext);

  RenderContext* renderContext = gameContext->getRenderContext();
  renderable->cleanUp(renderContext);
  renderContext->getRenderableSet()->removeRenderable(renderable->getID());
  renderable.reset();
  }

void BotPortal::destroyThis(GameContext* gameContext)
  {
  setHealthPoints(0);
  TOGameContext::cast(gameContext)->removeBotPortal(getID());
  }

void BotPortal::setPortalSpawnDef(std::shared_ptr<BotPortalSpawnDef> spawnDef)
  {
  this->spawnDef = spawnDef;
  spawnTimer.setTimeOut(spawnDef->timeBetweenSpawns);
  spawnTimer.reset();
  spawnIndex = -1;
  }

void BotPortal::doSpawning(GameContext* gameContext)
  {
  if (spawnDef && spawnTimer.incrementTimer(gameContext->getDeltaTime()))
    {
    if (spawnIndex == -1)
      {
      //  start spawning units
      spawnTimer.setTimeOut(200);
      spawnIndex = 0;
      }
    else if (spawnIndex < spawnDef->spawnsIDs.size())
      {
      //  spawn the next unit
      Vector3D spawnPos = getPosition() + Vector3D(0, 0, 0.5);
      TOGameContext::cast(gameContext)->createBot(spawnDef->spawnsIDs[spawnIndex], spawnPos);
      ++spawnIndex;
      }
    else
      {
      //  stop spawning units
      spawnTimer.setTimeOut(spawnDef->timeBetweenSpawns);
      spawnIndex = -1;
      }
    spawnTimer.reset();
    }
  }
