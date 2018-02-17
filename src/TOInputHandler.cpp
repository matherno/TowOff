//
// Created by matt on 3/12/17.
//

#include <GameSystem/InputCodes.h>
#include "TOInputHandler.h"
#include "TOGameContext.h"
#include "TowerFactory.h"

TOInputHandler::TOInputHandler(uint id, const Vector3D& position, float zoomOffset, float yaw, float pitch)
  : InputHandler(id), position(position), zoomOffset(zoomOffset), yaw(yaw), pitch(pitch)
  {
  }

void TOInputHandler::refreshCamera(Camera* camera) const
  {
  *camera->getWorldToCamera() = mathernogl::matrixTranslate(position * -1) * rotationMatrix * mathernogl::matrixTranslate(0, 0, -zoomOffset);
  camera->setValid(false);
  }

void TOInputHandler::onAttached(GameContext* gameContext)
  {
  Camera* camera = gameContext->getCamera();
  refreshRotationMatrix();
  refreshCamera(camera);
  *camera->getCameraToClip() = mathernogl::matrixPerspective(1, gameContext->getRenderContext()->getWindow()->getAspectRatio(), -1, -1000);
  camera->setValid(false);
  }

void TOInputHandler::onDetached(GameContext* gameContext)
  {

  }

bool TOInputHandler::onKeyHeld(GameContext* gameContext, uint key)
  {
  char character = getCharFromKeyCode(key);
  Vector3D translation(0);
  float rotation = 0;
  float speed = panSpeed * gameContext->getDeltaTime() * 0.001f;
  float rotateSpeed = yawSpeed * gameContext->getDeltaTime() * 0.001f;

  switch(character)
    {
    case 'W':
      translation.set(0, 0, -speed);
      break;
    case 'A':
      translation.set(-speed, 0, 0);
      break;
    case 'S':
      translation.set(0, 0, speed);
      break;
    case 'D':
      translation.set(speed, 0, 0);
      break;
    case 'Q':
      rotation = rotateSpeed;
      break;
    case 'E':
      rotation = -rotateSpeed;
      break;
    }

  if (translation.magnitude() > 0 || rotation != 0)
    {
    if (rotation != 0)
      {
      yaw += rotation;
      refreshRotationMatrix();
      }
    translation *= mathernogl::matrixYaw(yaw);
    position += translation;
    refreshCamera(gameContext->getCamera());
    return true;
    }
  return false;
  }

void TOInputHandler::refreshRotationMatrix()
  {
  rotationMatrix = mathernogl::matrixYaw(-yaw) * mathernogl::matrixPitch(-pitch);
  }

bool TOInputHandler::onKeyPressed(GameContext* gameContext, uint key)
  {
  switch(getCharFromKeyCode(key))
    {
    case 'P':
      paused = !paused;
      gameContext->setPaused(paused);
      return true;
    }
  return false;
  }

bool TOInputHandler::onMousePressed(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  TOGameContext* toGameContext = TOGameContext::cast(gameContext);
  HUDHandler* hudHandler = toGameContext->getHUDHandler();

  if (gameContext->getBoundingBoxManager()->boundingBoxPicked())
    {
    uint pickedTowerID = (uint)gameContext->getBoundingBoxManager()->getPickedBoundingBoxMeta();
    hudHandler->setTowerFocused(toGameContext->getTower(pickedTowerID));
    return true;
    }

  hudHandler->setTowerFocused(nullptr);
  return false;
  }

bool TOInputHandler::onMouseScroll(GameContext* gameContext, double scrollOffset, uint mouseX, uint mouseY)
  {
  zoomOffset -= scrollOffset * zoomSpeed;
  zoomOffset = mathernogl::clampf(zoomOffset, minZoom, maxZoom);
  refreshCamera(gameContext->getCamera());
  return true;
  }
