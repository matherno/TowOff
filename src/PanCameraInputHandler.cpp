//
// Created by matt on 3/12/17.
//

#include <src/GameSystem/InputCodes.h>
#include "PanCameraInputHandler.h"
#include "TOGameContext.h"

PanCameraInputHandler::PanCameraInputHandler(uint id, const Vector3D& position, float zoomOffset, float yaw, float pitch)
  : InputHandler(id), position(position), zoomOffset(zoomOffset), yaw(yaw), pitch(pitch)
  {
  }

void PanCameraInputHandler::refreshCamera(Camera* camera) const
  {
  *camera->getWorldToCamera() = mathernogl::matrixTranslate(position * -1) * rotationMatrix * mathernogl::matrixTranslate(0, 0, -zoomOffset);
  camera->setValid(false);
  }

void PanCameraInputHandler::onAttached(GameContext* gameContext)
  {
  Camera* camera = gameContext->getCamera();
  refreshRotationMatrix();
  refreshCamera(camera);
  *camera->getCameraToClip() = mathernogl::matrixPerspective(1, (float)gameContext->getRenderContext()->getWindow()->getAspectRatio(), -1, -1000);
  camera->setValid(false);
  }

void PanCameraInputHandler::onDetached(GameContext* gameContext)
  {

  }

bool PanCameraInputHandler::onKeyHeld(GameContext* gameContext, uint key)
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

void PanCameraInputHandler::refreshRotationMatrix()
  {
  rotationMatrix = mathernogl::matrixYaw(-yaw) * mathernogl::matrixPitch(-pitch);
  }

bool PanCameraInputHandler::onKeyPressed(GameContext* gameContext, uint key)
  {
  switch(getCharFromKeyCode(key))
    {
    case 'P':
      paused = !paused;
      gameContext->setPaused(paused);
      break;
    }
  }

bool PanCameraInputHandler::onMousePressed(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  if (activePlayer != 0 && button == MOUSE_LEFT)
    {
    TOGameContext* toGameContext = TOGameContext::cast(gameContext);
    Vector3D terrainPos = toGameContext->terrainHitTest(mouseX, mouseY);
    TowerPtr tower = toGameContext->createBasicTower(terrainPos);
    tower->setPlayerNum(activePlayer);
    return true;
    }
  else if (button == MOUSE_RIGHT)
    {
    activePlayer = 0;
    return true;
    }
  return false;
  }

