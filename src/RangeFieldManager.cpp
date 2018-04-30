//
// Created by matt on 25/02/18.
//

#include "RangeFieldManager.h"

RangeFieldManager::RangeFieldManager(uint id) : GameActor(id)
  {}

void RangeFieldManager::onAttached(GameContext* gameContext)
  {
  lineStripsRenderable.reset(new RenderableLineStrips(gameContext->getRenderContext()->getNextRenderableID(), false, DRAW_STAGE_TRANSPARENT));
  lineStripsRenderable->initialise(gameContext->getRenderContext());
  lineStripsRenderable->setLineWidth(1);
  gameContext->getRenderContext()->getRenderableSet()->addRenderable(lineStripsRenderable);
  }

void RangeFieldManager::onUpdate(GameContext* gameContext)
  {
  float scaling = (float)sin(gameContext->getGameTime() * 0.003);
  scaling *= 0.3;
  scaling += 1.2;
  lineStripsRenderable->getTransform()->setIdentityMatrix();
  lineStripsRenderable->getTransform()->scale(1, scaling, 1);
  }

void RangeFieldManager::onDetached(GameContext* gameContext)
  {
  lineStripsRenderable->cleanUp(gameContext->getRenderContext());
  gameContext->getRenderContext()->getRenderableSet()->removeRenderable(lineStripsRenderable->getID());
  }

#define NUM_SECTORS 60

uint RangeFieldManager::createRangeField(const Vector3D& position, float radius, const Vector3D& colour)
  {
  uint id = nextRangeFieldID++;
  rangeFieldLineStrips.add(LineStripIDs(), id);
  LineStripIDs* lineStripIDs = rangeFieldLineStrips.getPtr(id);

  Vector3D centre = position;
  lineStripsRenderable->startLineStrip(colour, transparency);
  lineStripsRenderable->addXZPlaneCircle(centre, radius, NUM_SECTORS);
  lineStripIDs->push_back(lineStripsRenderable->finishLineStrip());

  centre += Vector3D(0, 0.3, 0);
  lineStripsRenderable->startLineStrip(colour, transparency * 0.75f);
  lineStripsRenderable->addXZPlaneCircle(centre, radius, NUM_SECTORS);
  lineStripIDs->push_back(lineStripsRenderable->finishLineStrip());
//
  centre += Vector3D(0, 0.3, 0);
  lineStripsRenderable->startLineStrip(colour, transparency * 0.5f);
  lineStripsRenderable->addXZPlaneCircle(centre, radius, NUM_SECTORS);
  lineStripIDs->push_back(lineStripsRenderable->finishLineStrip());

  return id;
  }

void RangeFieldManager::removeRangeField(uint id)
  {
  if (rangeFieldLineStrips.contains(id))
    {
    const LineStripIDs& lineStripIDs = rangeFieldLineStrips.get(id);
    for (uint lineStripID : lineStripIDs)
      lineStripsRenderable->removeLineStrip(lineStripID);
    rangeFieldLineStrips.remove(id);
    }
  }
