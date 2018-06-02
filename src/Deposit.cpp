//
// Created by matt on 19/05/18.
//

#include "Deposit.h"
#include "Resources.h"

#define UPDATE_TIME 1000

Deposit::Deposit(uint id) : GameActor(id)
  {}

static const std::vector<string> possibleModelFiles =
  {
    MESH_CRYSTALA,
    MESH_CRYSTALB,
    MESH_CRYSTALC,
    MESH_CRYSTALD,
    MESH_CRYSTALE,
    MESH_CRYSTALF,
  };


void Deposit::onAttached(GameContext* gameContext)
  {
  ASSERT(!possibleModelFiles.empty(), "No models to use???")
  const int modelIdx = mathernogl::RandomGenerator::randomInt(0, possibleModelFiles.size() - 1);
  ASSERT(modelIdx >= 0 && modelIdx < possibleModelFiles.size(), "Invalid model index!");

  RenderContext* renderContext = gameContext->getRenderContext();
  modelTexture = renderContext->getSharedTexture(IMAGE_TEXTURE_PALETTE);
  modelRenderable.reset(new RenderableMesh(renderContext->getNextRenderableID(), DRAW_STAGE_TRANSPARENT));
  modelRenderable->setMeshStorage(renderContext->getSharedMeshStorage(possibleModelFiles[modelIdx]));
  modelRenderable->setDrawStyleTexture(modelTexture);
  modelRenderable->setTransparency(0.3);
  randomiseTransform();
  modelRenderable->initialise(renderContext);
  renderContext->getRenderableSet()->addRenderable(modelRenderable);

  ambientParticles.reset(new ParticleSystem(gameContext->getNextActorID(), false));
  ambientParticles->setParticleDirectionRandom();
  ambientParticles->setParticleSpawnSphere(1.5);
  ambientParticles->setInitVelocity(0.0002);
  ambientParticles->setGravityAccel(0);
  ambientParticles->setParticleSize(0.6);
  ambientParticles->setDepthTesting(false);
  ambientParticles->setAdditiveBlending(true);
  ambientParticles->addTextureAtlas(renderContext->getSharedTexture(IMAGE_CRYSTALAMBIENT_SHEET1));
  ambientParticles->addTextureAtlas(renderContext->getSharedTexture(IMAGE_CRYSTALAMBIENT_SHEET2));
  ambientParticles->setTextureAtlasSize(3, 3);
  ambientParticles->setTextureColourMixFactor(0);
  ambientParticles->addEmitter(position, [this](){ return false; }, Vector3D(), 0.3);
  ambientParticles->setTimeAlive(4000);
  gameContext->addActor(ambientParticles);

  updateTimer.setTimeOut(UPDATE_TIME);
  }


void Deposit::onUpdate(GameContext* gameContext)
  {
  if (updateTimer.incrementTimer(gameContext->getDeltaTime()))
    {
    const float energyFactor = (float) storedEnergy / (float) maxEnergy;
    if (energyFactor > 0.1)
      {
      const long timeBetweenSpawns = mathernogl::RandomGenerator::randomInt(600, 1000);
      ambientParticles->setTimeBetweenSpawns(timeBetweenSpawns + (1.0f - energyFactor) * 800, true);
      }
    modelRenderable->setDrawStyleTexture(modelTexture, (1.0f - energyFactor) * 0.9f, Vector3D(0.01, 0, 0.01));
    updateTimer.reset();
    storedEnergy += energyRegenPerSec;
    }
  }

void Deposit::onDetached(GameContext* gameContext)
  {
  modelRenderable->cleanUp(gameContext->getRenderContext());
  gameContext->getRenderContext()->getRenderableSet()->removeRenderable(modelRenderable->getID());
  gameContext->removeActor(ambientParticles->getID());
  modelRenderable.reset();
  ambientParticles.reset();
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