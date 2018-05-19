//
// Created by matt on 28/01/18.
//

#include <RenderSystem/RenderableLines.h>
#include "TowerFactory.h"
#include "Tower.h"
#include "Resources.h"
#include "DiscreteWeapon.h"
#include "TowerFunctionalities.h"
#include "UnderConstructTower.h"
#include "TimeToLiveActor.h"
#include "Projectile.h"

#define TOWER_HOMEBASE        0
#define TOWER_BASIC           1
#define TOWER_PYLON           2
#define TOWER_MINER           3
#define TOWER_MACHINEGUN      4
#define TOWER_MORTAR          5
#define TOWER_MISSILELAUNCHER 6
#define TOWER_SNIPER          7

const std::map<uint, TowerType> towerTypes =
  {
  //{   id,         TowerType{     name,                  icon image file,        base mesh file,                turret mesh file,                 connect offset,        connect radius,  hit radius}},
  {TOWER_HOMEBASE,     TowerType{"Home Base",           IMAGE_ICON_HOME_BASE,    MESH_HOME_BASE,                "",                              Vector3D(-0.61, 5.12, 0.61),  10,             2.7}},
  {TOWER_BASIC,        TowerType{"Basic Tower A",       IMAGE_ICON_BASIC_TOWER,  MESH_BASIC_TOWER_BASE,         MESH_BASIC_TOWER_TURRET,         Vector3D(0, 1, 0),            10,             0.9}},
  {TOWER_PYLON,        TowerType{"Pylon",               IMAGE_ICON_PYLON,        MESH_PYLON,                    "",                              Vector3D(0, 4.02, 0),         30,             1.5}},
  {TOWER_MINER,        TowerType{"Miner",               IMAGE_ICON_MINER,        MESH_MINER_BASE,               MESH_MINER_TURRET,               Vector3D(0, 2.61, 0),         10,             1.7}},
  {TOWER_MACHINEGUN,   TowerType{"Machine Gun Tower",   IMAGE_ICON_MACHINEGUN,   MESH_MACHINEGUN_TOWER_BASE,    MESH_MACHINEGUN_TOWER_TURRET,    Vector3D(0, 1, 0),            10,             1.2}},
  {TOWER_MORTAR,       TowerType{"Mortar Tower",        IMAGE_ICON_MORTAR,       MESH_MORTAR_BASE,              MESH_MORTAR_TURRET,              Vector3D(0, 1, 0),            10,             1.2}},
  {TOWER_MISSILELAUNCHER, TowerType{"Missile Launcher", IMAGE_ICON_MISSILELAUNCHER, MESH_MISSILELAUNCHER_BASE,  MESH_MISSILELAUNCHER_TURRET,     Vector3D(0, 1, 0),            10,             1.4}},
  {TOWER_SNIPER,          TowerType{"Sniper Tower",     IMAGE_ICON_SNIPER,       MESH_SNIPER_BASE,              MESH_SNIPER_TURRET,              Vector3D(0, 1, 0),            10,             1.4}},
  };

const std::map<uint, TowerType>* TowerFactory::getTowerTypeMap()
  {
  return &towerTypes;
  }

const TowerType* TowerFactory::getTowerType(uint towerType)
  {
  ASSERT(towerTypes.count(towerType) > 0, "Invalid tower type number");
  return &towerTypes.at(towerType);
  }

TowerPtr TowerFactory::createTower(uint towerType, uint id, const Vector3D& position)
  {
  switch (towerType)
    {
    case TOWER_HOMEBASE:
      return createHomeBase(id, towerType, position);
    case TOWER_BASIC:
      return createBasicTower(id, towerType, position);
    case TOWER_PYLON:
      return createPylon(id, towerType, position);
    case TOWER_MINER:
      return createMiner(id, towerType, position);
    case TOWER_MACHINEGUN:
      return createMachineGunTower(id, towerType, position);
    case TOWER_MORTAR:
      return createMortar(id, towerType, position);
    case TOWER_MISSILELAUNCHER:
      return createMissileLauncher(id, towerType, position);
    case TOWER_SNIPER:
      return createSniperTower(id, towerType, position);
    }
  return nullptr;
  }

void setCommonTowerParameters(TowerPtr tower, uint towerType)
  {
  const TowerType* type = TowerFactory::getTowerType(towerType);
  tower->setBaseModelFilePath(type->baseMeshFilePath);
  tower->setTurretModelFilePath(type->turretMeshFilePath);
  tower->setConnectOffset(type->connectOffset);
  tower->setConnectRadius(type->connectRadius);
  tower->setTargetOffset(Vector3D(0, 0.5, 0));
  tower->setHitRadius(type->hitRadius);
  }

TowerPtr TowerFactory::createUnderConstructTower(uint towerType, uint id, const Vector3D& position)
  {
  TowerPtr tower(new UnderConstructTower(id, towerType));
  tower->setPosition(position);
  setCommonTowerParameters(tower, towerType);
  return tower;
  }

TowerPtr TowerFactory::createBasicTower(uint id, uint towerType, const Vector3D& position)
  {
  std::unique_ptr<DiscreteWeapon> weapon(new DiscreteWeapon());
  weapon->setCooldownTime(1000);
  weapon->setOnShootFunction([](GameContext* context, const Vector3D& shootPos, const Vector3D& targetPos)
                               {
                               RenderContext* renderContext = context->getRenderContext();
                               RenderableLines* line = new RenderableLines(renderContext->getNextRenderableID());
                               line->initialise(renderContext);
                               line->addLine(shootPos, targetPos, Vector3D(0.7, 0.1, 0.1));

                               RenderablePtr linePtr(line);
                               renderContext->getRenderableSet()->addRenderable(linePtr);
                               TimeToLiveActor* actor = new TimeToLiveActor(context->getNextActorID(), 60);
                               actor->addRenderable(linePtr);
                               context->addActor(GameActorPtr(actor));
                               });

  TowerFunctionalityCombat* function = new TowerFunctionalityCombat();
  function->setShootOffset(Vector3D(0, 1.45, -1));
  function->setWeapon(std::move(weapon));

  TowerPtr tower(new Tower(id, towerType, std::move(TowerFunctionalityPtr(function))));
  tower->setPosition(position);
  setCommonTowerParameters(tower, towerType);
  return tower;
  }

TowerPtr TowerFactory::createHomeBase(uint id, uint towerType, const Vector3D& position)
  {
  TowerFunctionalityStorage* function = new TowerFunctionalityStorage();

  TowerPtr tower(new Tower(id, towerType, std::move(TowerFunctionalityPtr(function))));
  tower->setPosition(position);
  tower->setMaxEnergy(20000);
  tower->storeEnergy(tower->getMaxEnergy());
  setCommonTowerParameters(tower, towerType);
  return tower;
  }

TowerPtr TowerFactory::createPylon(uint id, uint towerType, const Vector3D& position)
  {
  TowerFunctionalityRelay* function = new TowerFunctionalityRelay();

  TowerPtr tower(new Tower(id, towerType, std::move(TowerFunctionalityPtr(function))));
  tower->setPosition(position);
  setCommonTowerParameters(tower, towerType);
  return tower;
  }

TowerPtr TowerFactory::createMiner(uint id, uint towerType, const Vector3D& position)
  {
  TowerFunctionalityMiner* function = new TowerFunctionalityMiner();
  function->setEnergyTransferRate(60);
  function->addMiningBeamOffset(Vector3D(0.7, 1.6, 0));
  function->addMiningBeamOffset(Vector3D(-0.7, 1.6, 0));

  TowerPtr tower(new Tower(id, towerType, std::move(TowerFunctionalityPtr(function))));
  tower->setPosition(position);
  setCommonTowerParameters(tower, towerType);
  return tower;
  }

TowerPtr TowerFactory::createMachineGunTower(uint id, uint towerType, const Vector3D& position)
  {
  std::unique_ptr<DiscreteWeapon> weapon(new DiscreteWeapon());
  weapon->setCooldownTime(100);
  weapon->setDamagePerShot(3);
  weapon->setOnShootFunction(
    [](GameContext* context, const Vector3D& shootPos, const Vector3D& targetPos)
     {
     const Vector3D shootDirection = Vector3D(targetPos - shootPos).getUniform();
     const Vector3D turretRight = mathernogl::crossProduct(shootDirection, Vector3D(0, 1, 0));
     static const float gunsOffset = 0.71f;
     static const float targetOffset = 0.1f;
     static const Vector3D beamColour = Vector3D(0.3, 0.2, 0.8);

     RenderContext* renderContext = context->getRenderContext();
     RenderableLines* line = new RenderableLines(renderContext->getNextRenderableID());
     line->initialise(renderContext);
     line->addLine(
       shootPos + turretRight * gunsOffset,
       targetPos + turretRight * targetOffset, beamColour);
     line->addLine(
       shootPos - turretRight * gunsOffset,
       targetPos - turretRight * targetOffset, beamColour);
     line->setLineWidth(1);

     RenderablePtr linePtr(line);
     renderContext->getRenderableSet()->addRenderable(linePtr);
     TimeToLiveActor* actor = new TimeToLiveActor(context->getNextActorID(), 60);
     actor->addRenderable(linePtr);
     context->addActor(GameActorPtr(actor));
     });

  TowerFunctionalityCombat* function = new TowerFunctionalityCombat();
  function->setShootOffset(Vector3D(0, 1.6, -1.56));
  function->setWeapon(std::move(weapon));

  TowerPtr tower(new Tower(id, towerType, std::move(TowerFunctionalityPtr(function))));
  tower->setPosition(position);
  setCommonTowerParameters(tower, towerType);
  return tower;
  }

TowerPtr TowerFactory::createMortar(uint id, uint towerType, const Vector3D& position)
  {
  std::unique_ptr<DiscreteWeapon> weapon(new DiscreteWeapon());
  weapon->setCooldownTime(3000);
  weapon->setDamagePerShot(0);
  weapon->setOnShootFunction(
    [](GameContext* context, const Vector3D& shootPos, const Vector3D& targetPos)
      {
      static const float inclination = 30;
      static const float sinI = (float) sin(mathernogl::degToRad(inclination));
      static const float cosI = (float) cos(mathernogl::degToRad(inclination));
      static const float cos2I = cosI * cosI;
      static const Vector3D inclinationDir =
        Vector3D(0, 0, -1) * mathernogl::matrixRotate(Vector3D(1, 0, 0), inclination);
      static const float gravity = 15;

      const Vector3D posToTarget = Vector3D(targetPos - shootPos);
      float azimuth = (float) mathernogl::ccwAngleBetween(Vector2D(0, -1), Vector2D(posToTarget.x, posToTarget.z).getUniform());
      const Vector3D shootDirection =
        inclinationDir * mathernogl::matrixRotate(Vector3D(0, 1, 0), azimuth);

      const float vertDisp = targetPos.y - shootPos.y;
      const float horizDisp = (posToTarget + Vector3D(0, 1, 0) * vertDisp).magnitude();
      float initialSpeed = gravity * horizDisp * horizDisp;
      initialSpeed /= (2 * cosI * sinI * horizDisp) - (2 * cos2I * vertDisp);
      initialSpeed = sqrtf(initialSpeed);

      MortarProjectile* projectile = new MortarProjectile(context->getNextActorID());
      projectile->setScale(1.5);
      projectile->setMeshFilePath(MESH_MORTAR_PROJECTILE);
      projectile->setTextureFilePath(IMAGE_TEXTURE_PALETTE);
      projectile->setDragEffect(0.2);
      projectile->setTimeToLive(5000);
      projectile->setPosition(shootPos);
      projectile->setVelocity(shootDirection * initialSpeed);
      projectile->setAcceleration(Vector3D(0, -1, 0) * gravity);
      projectile->setDamageRadius(6);
      projectile->setDamageAmount(20);
      context->addActor(ProjectilePtr(projectile));
      }
  );

  TowerFunctionalityCombat* function = new TowerFunctionalityCombat();
  function->setShootOffset(Vector3D(0, 2.6, -1.3));
  function->setWeapon(std::move(weapon));

  TowerPtr tower(new Tower(id, towerType, std::move(TowerFunctionalityPtr(function))));
  tower->setPosition(position);
  setCommonTowerParameters(tower, towerType);
  return tower;
  }


TowerPtr TowerFactory::createMissileLauncher(uint id, uint towerType, const Vector3D& position)
  {
  std::unique_ptr<TrackingWeapon> weapon(new TrackingWeapon());
  weapon->setCooldownTime(1000);
  weapon->setBurstParams(4, 100);
  weapon->setDamagePerShot(0);
  weapon->setCreateTrackerFunction(
    [](GameContext* context, const Vector3D& shootPos, TowerTargetPtr target)
      {
      std::shared_ptr<TrackingMissileProjectile> projectile (new TrackingMissileProjectile(context->getNextActorID()));
      projectile->setScale(1.5);
      projectile->setMeshFilePath(MESH_MORTAR_PROJECTILE);
      projectile->setTextureFilePath(IMAGE_TEXTURE_PALETTE);
      projectile->setTimeToLive(5000);
      projectile->setDamageRadius(3);
      projectile->setDamageAmount(10);
      projectile->setMissileSpeed(2);
      projectile->updateTargetPosition(target->getTargetPosition());

      const Vector3D shootDirection = Vector3D(target->getTargetPosition() - shootPos).getUniform();
      const Vector3D turretRight = mathernogl::crossProduct(shootDirection, Vector3D(0, 1, 0));
      const Vector3D turretUp = mathernogl::crossProduct(turretRight, shootDirection);
      const float shootPosXOffset = mathernogl::RandomGenerator::randomFloat(0.5, 1.1);
      const float shootPosYOffset = mathernogl::RandomGenerator::randomFloat(-0.3, 0.3);
      const bool rightSide = (bool) mathernogl::RandomGenerator::randomInt(0, 1);

      projectile->setPosition(shootPos + turretRight * shootPosXOffset * (rightSide ? 1 : -1) + turretUp * shootPosYOffset);
      context->addActor(projectile);
      return projectile;
      }
  );

  TowerFunctionalityCombat* function = new TowerFunctionalityCombat();
  function->setShootOffset(Vector3D(0, 1.8, -0.9));
  function->setWeapon(std::move(weapon));

  TowerPtr tower(new Tower(id, towerType, std::move(TowerFunctionalityPtr(function))));
  tower->setPosition(position);
  setCommonTowerParameters(tower, towerType);
  return tower;
  }

TowerPtr TowerFactory::createSniperTower(uint id, uint towerType, const Vector3D& position)
  {
  std::unique_ptr<DiscreteWeapon> weapon(new DiscreteWeapon());
  weapon->setCooldownTime(3000);
  weapon->setDamagePerShot(50);
  weapon->setOnShootFunction(
    [](GameContext* context, const Vector3D& shootPos, const Vector3D& targetPos)
      {
      const Vector3D beamColour = Vector3D(0.0, 0.1, 0.3);
      RenderContext* renderContext = context->getRenderContext();
      RenderableLines* line = new RenderableLines(renderContext->getNextRenderableID());
      line->initialise(renderContext);
      line->addLine(shootPos, targetPos, beamColour);
      line->setLineWidth(3);

      const long beamTimeAlive = 200;
      RenderablePtr linePtr(line);
      renderContext->getRenderableSet()->addRenderable(linePtr);
      TimeToLiveActor* actor = new TimeToLiveActor(context->getNextActorID(), beamTimeAlive);
      actor->addRenderable(linePtr);
      context->addActor(GameActorPtr(actor));

      const long particleTimeAlive = 100;
      ParticleSystemPtr particles(new ParticleSystem(context->getNextActorID(), false));
      particles->setGravityAccel(0);
      particles->setTimeBetweenSpawns(3);
      particles->setInitVelocity(0.002);
      particles->setTimeAlive(particleTimeAlive);
      particles->setParticleSpawnLine(shootPos, targetPos);
      particles->setParticleDirectionRandom();
      particles->setDepthTesting(true);
      particles->setAdditiveBlending(false);
      particles->addTextureAtlas(renderContext->getSharedTexture(IMAGE_BLUEENERGY_SHEET1));
      particles->addTextureAtlas(renderContext->getSharedTexture(IMAGE_BLUEENERGY_SHEET2));
      particles->addTextureAtlas(renderContext->getSharedTexture(IMAGE_BLUEENERGY_SHEET3));
      particles->setTextureAtlasSize(3, 3);
      particles->setTextureColourMixFactor(0);
      particles->addEmitter(Vector3D(0), beamTimeAlive + particleTimeAlive, beamColour, 0.3);
      particles->addEmitter(Vector3D(0), beamTimeAlive,                     beamColour, 0.5);
      context->addActor(particles);

      actor = new TimeToLiveActor(context->getNextActorID(), beamTimeAlive + particleTimeAlive + particleTimeAlive);
      actor->addGameActor(particles);
      context->addActor(GameActorPtr(actor));
      });

  TowerFunctionalityCombat* function = new TowerFunctionalityCombat();
  function->setShootOffset(Vector3D(0, 2.48, -0.68));
  function->setWeapon(std::move(weapon));

  TowerPtr tower(new Tower(id, towerType, std::move(TowerFunctionalityPtr(function))));
  tower->setPosition(position);
  setCommonTowerParameters(tower, towerType);
  return tower;
  }

float TowerFactory::getTowerRange(uint towerType)
  {
  Tower::TowerFunction function = getTowerFunction(towerType);
  if (function == Tower::relay)
    return getRelayPowerRange(towerType);
  else if (function == Tower::combat)
    return getCombatRange(towerType);
  else if (function == Tower::miner)
    return getMinerRange(towerType);
  return 0;
  }

float TowerFactory::getRelayPowerRange(uint towerType)
  {
  switch (towerType)
    {
    case TOWER_PYLON:
      return 20;
    }
  return 0;
  }

float TowerFactory::getCombatRange(uint towerType)
  {
  switch (towerType)
    {
    case TOWER_BASIC:
      return 15;
    case TOWER_MACHINEGUN:
      return 15;
    case TOWER_MORTAR:
      return 35;
    case TOWER_MISSILELAUNCHER:
      return 20;
    case TOWER_SNIPER:
      return 35;
    }
  return 0;
  }

float TowerFactory::getCombatMinRange(uint towerType)
  {
  switch (towerType)
    {
    case TOWER_MACHINEGUN:
    case TOWER_MISSILELAUNCHER:
      return 5;
    case TOWER_MORTAR:
    case TOWER_SNIPER:
      return 15;
    }
  return 0;
  }

float TowerFactory::getMinerRange(uint towerType)
  {
  switch (towerType)
    {
    case TOWER_MINER:
      return 10;
    }
  return 0;
  }

void TowerFactory::createTowerBoundingBoxes(uint towerType, const Vector3D& position, std::list<BoundingBoxPtr>* boundingBoxes)
  {
  auto AddBoundingBox = [position, boundingBoxes](const Vector3D& min, const Vector3D& max)
    {
    BoundingBox* bb = new BoundingBox(position + min, position + max);
    boundingBoxes->push_back(BoundingBoxPtr(bb));
    };

  switch (towerType)
    {
    case TOWER_BASIC:
      AddBoundingBox(Vector3D(-0.7, 0, -0.7), Vector3D(0.7, 1.3, 0.7));
      break;
    case TOWER_PYLON:
      AddBoundingBox(Vector3D(-0.8, 0, -0.8), Vector3D(0.8, 4.1, 0.8));
      break;
    case TOWER_MINER:
      AddBoundingBox(Vector3D(-1.4, 0, -1.4), Vector3D(1.4, 0.5, 1.4));
      AddBoundingBox(Vector3D(-0.8, 0, -0.45), Vector3D(0.8, 2.8, 0.45));
      break;
    case TOWER_HOMEBASE:
      AddBoundingBox(Vector3D(-2.6, 0, -2.6), Vector3D(2.6, 0.65, 2.6));
      AddBoundingBox(Vector3D(-1.3, 0, -1.3), Vector3D(1.3, 5.5, 1.3));
      break;
    case TOWER_MACHINEGUN:
      AddBoundingBox(Vector3D(-1.0, 0, -1.0), Vector3D(1.0, 1.8, 1.0));
      break;
    case TOWER_MORTAR:
      AddBoundingBox(Vector3D(-1.0, 0, -1.0), Vector3D(1.0, 2.0, 1.0));
      break;
    case TOWER_MISSILELAUNCHER:
      AddBoundingBox(Vector3D(-1.5, 0, -1.5), Vector3D(1.5, 2.1, 1.5));
      break;
    case TOWER_SNIPER:
      AddBoundingBox(Vector3D(-1.5, 0, -1.5), Vector3D(1.5, 3.2, 1.5));
      break;
    }
  }

Tower::TowerFunction TowerFactory::getTowerFunction(uint towerType)
  {
  switch (towerType)
    {
    case TOWER_BASIC:
    case TOWER_MACHINEGUN:
    case TOWER_MORTAR:
    case TOWER_MISSILELAUNCHER:
    case TOWER_SNIPER:
      return Tower::combat;
    case TOWER_PYLON:
      return Tower::relay;
    case TOWER_MINER:
      return Tower::miner;
    case TOWER_HOMEBASE:
      return Tower::storage;
    default:
      return Tower::combat;
    }
  }

bool TowerFactory::canConstructTowerType(uint towerType)
  {
  return towerTypes.count(towerType) > 0 && towerType != getStartTowerTypeID();
  }

uint TowerFactory::getStartTowerTypeID()
  {
  return TOWER_HOMEBASE;
  }



