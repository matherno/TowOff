//
// Created by matt on 3/12/17.
//

#include <RenderSystem/RenderableMesh.h>
#include "Tower.h"
#include "TOGameContext.h"
#include "Resources.h"

Tower::Tower(uint id, uint towerType, TowerFunctionalityPtr functionality)
  : GameActor(id), towerType(towerType), functionality(std::move(functionality))
  {
  }

void Tower::onAttached(GameContext* gameContext)
  {
  RenderContext* renderContext = gameContext->getRenderContext();
  TexturePtr paletteTexture = renderContext->getSharedTexture(IMAGE_TEXTURE_PALETTE);

  //  tower base renderable
  if (!baseModelFile.empty())
    {
    MeshStoragePtr meshStorage = renderContext->getSharedMeshStorage(baseModelFile);
    if (meshStorage)
      {
      RenderableMesh* renderable = new RenderableMesh(renderContext->getNextRenderableID());
      renderable->setMeshStorage(meshStorage);
      renderable->initialise(renderContext);
      renderable->getTransform()->translate(position);
      renderable->setDrawStyleTexture(paletteTexture);
      towerBase = RenderablePtr(renderable);
      renderContext->getRenderableSet()->addRenderable(towerBase);
      }
    }

  //  tower turret renderable
  if (!turretModelFile.empty())
    {
    MeshStoragePtr meshStorage = renderContext->getSharedMeshStorage(turretModelFile);
    if (meshStorage)
      {
      RenderableMesh* renderable = new RenderableMesh(renderContext->getNextRenderableID());
      renderable->setMeshStorage(meshStorage);
      renderable->initialise(renderContext);
      renderable->getTransform()->translate(position);
      renderable->setDrawStyleTexture(paletteTexture);
      towerTurret = RenderablePtr(renderable);
      renderContext->getRenderableSet()->addRenderable(towerTurret);
      }
    }

  if (functionality)
    functionality->onAttached(this, gameContext);
  }

void Tower::onUpdate(GameContext* gameContext)
  {
  TOGameContext* toGameContext = TOGameContext::cast(gameContext);
  if (showDamageEffect)
    {
    toGameContext->doTowerDamageEffect(this);
    showDamageEffect = false;
    }

  if (!isAlive())
    {
    toGameContext->removeTower(getID());
    return;
    }

  if (functionality)
    functionality->onUpdate(this, gameContext);
  }

void Tower::onDetached(GameContext* gameContext)
  {
  if (functionality)
    functionality->onDetached(this, gameContext);

  if (towerBase)
    {
    gameContext->getRenderContext()->getRenderableSet()->removeRenderable(towerBase->getID());
    towerBase->cleanUp(gameContext->getRenderContext());
    towerBase.reset();
    }
  if (towerTurret)
    {
    gameContext->getRenderContext()->getRenderableSet()->removeRenderable(towerTurret->getID());
    towerTurret->cleanUp(gameContext->getRenderContext());
    towerTurret.reset();
    }
  }

bool Tower::inflictDamage(int damagePoints)
  {
  showDamageEffect = true;
  healthPoints -= damagePoints;
  return healthPoints <= 0;
  }

void Tower::setPosition(const Vector3D& pos)
  {
  position = pos;
  if (towerBase)
    towerBase->getTransform()->setTransformMatrix(mathernogl::matrixTranslate(pos));
  if (towerTurret)
    towerTurret->getTransform()->setTransformMatrix(mathernogl::matrixTranslate(pos));
  setTargetOffset(targetOffset);
  setConnectOffset(connectOffset);
  }

void Tower::setTargetOffset(const Vector3D& offset)
  {
  targetOffset = offset;
  targetPosition = position + targetOffset;
  }

void Tower::setConnectOffset(const Vector3D& offset)
  {
  connectOffset = offset;
  connectPosition = position + offset;
  }

void Tower::setMaxHealthPoints(int max)
  {
  maxHealthPoints = max;
  if (healthPoints > maxHealthPoints)
    healthPoints = maxHealthPoints;
  }

void Tower::setTurretRotation(const Vector3D& targetPos)
  {
  if (towerTurret)
    {
    Vector3D targetDirection = targetPos - position;
    targetDirection.y = 0;
    targetDirection.makeUniform();
    double angleBetween = mathernogl::ccwAngleBetween(Vector2D(0, -1), Vector2D(targetDirection.x, targetDirection.z));
    setTurretYRotation(angleBetween);
    }
  }

const Transform* Tower::getTurretTransform() const
  {
  if (towerTurret)
    return towerTurret->getTransform();
  return nullptr;
  }

void Tower::setTurretYRotation(double yRotation)
  {
  if (towerTurret)
    {
    turretRotation = yRotation;
    towerTurret->getTransform()->setIdentityMatrix();
    towerTurret->getTransform()->rotate(0, 1, 0, yRotation);
    towerTurret->getTransform()->translate(position);
    }
  }

double Tower::getTurretYRotation() const
  {
  return turretRotation;
  }

Tower::TowerFunction Tower::getFunction() const
  {
  if (functionality)
    return functionality->function;
  else
    return Tower::none;
  }

uint Tower::takeEnergy(uint amount, bool allOrNothing)
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

uint Tower::storeEnergy(uint amount)
  {
  if (amount + storedEnergy > maxEnergy)
    amount = maxEnergy - storedEnergy;
  storedEnergy += amount;
  return amount;
  }

void Tower::getTowerState(TowerState* state)
  {
  state->type = towerType;
  state->position = position;
  state->rotation = turretRotation;
  state->health = (uint)std::max(healthPoints, 0);
  state->energy = storedEnergy;
  state->underConstruction = isUnderConstruction();
  if(functionality)
    functionality->onGetTowerState(state);
  }

void Tower::setTowerState(const TowerState* state)
  {
  setPosition(state->position);
  setTurretYRotation(state->rotation);
  healthPoints = std::min((int)state->health, maxHealthPoints);
  storedEnergy = std::min(state->energy, maxEnergy);
  if(functionality)
    functionality->onSetTowerState(state);
  }


