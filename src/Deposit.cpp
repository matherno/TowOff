//
// Created by matt on 19/05/18.
//

#include "Deposit.h"
#include "Resources.h"

Deposit::Deposit(uint id) : GameActor(id)
  {}

static const std::vector<string> possibleModelFiles =
  {
    MESH_CRYSTALA,
    MESH_CRYSTALB,
    MESH_CRYSTALC,
  };

void Deposit::onAttached(GameContext* gameContext)
  {
  ASSERT(!possibleModelFiles.empty(), "No models to use???")
  const int modelIdx = mathernogl::RandomGenerator::randomInt(0, possibleModelFiles.size() - 1);
  ASSERT(modelIdx >= 0 && modelIdx < possibleModelFiles.size(), "Invalid model index!");

  RenderContext* renderContext = gameContext->getRenderContext();
  modelRenderable.reset(new RenderableMesh(renderContext->getNextRenderableID()));
  modelRenderable->setMeshStorage(renderContext->getSharedMeshStorage(possibleModelFiles[modelIdx]));
  modelRenderable->setDrawStyleTexture(renderContext->getSharedTexture(IMAGE_TEXTURE_PALETTE));
  randomiseTransform();
  modelRenderable->initialise(renderContext);
  renderContext->getRenderableSet()->addRenderable(modelRenderable);

  }

void Deposit::onUpdate(GameContext* gameContext)
  {
  }

void Deposit::onDetached(GameContext* gameContext)
  {
  modelRenderable->cleanUp(gameContext->getRenderContext());
  gameContext->getRenderContext()->getRenderableSet()->removeRenderable(modelRenderable->getID());
  modelRenderable.reset();
  }

void Deposit::setPosition(const Vector3D& position)
  {
  this->position = position;
  }

void Deposit::randomiseTransform()
  {
  using namespace mathernogl;
  static const float minScale = 0.5;
  static const float maxScale = 1;
  const float scale = RandomGenerator::randomFloat(minScale, maxScale);
  hitRadius *= scale;
  modelRenderable->setTransform(matrixScale(scale) *
                                  matrixRotate(0, 1, 0, RandomGenerator::randomFloat(0, 360)) *
                                  matrixTranslate(position));
  }

uint Deposit::takeEnergy(uint amount, bool allOrNothing)
  {
  if (amount > storedEnergy)
    {
    if (allOrNothing)
      return 0;
    else
      amount = storedEnergy;
    }
  storedEnergy -= amount;
  return amount;
  }