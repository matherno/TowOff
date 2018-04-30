//
// Created by matt on 3/12/17.
//

#include <GameSystem/InputCodes.h>
#include "TOInputHandler.h"
#include "TOGameContext.h"
#include "TowerFactory.h"
#include "SaveLoadDlg.h"

TOInputHandler::TOInputHandler(uint id, const Vector3D& focalPosition, float zoomOffset, float rotation, float pitch)
  : InputHandler(id), focalPosition(focalPosition), zoomOffset(zoomOffset), rotation(rotation), pitch(pitch)
  {
  }

void TOInputHandler::refreshCamera(Camera* camera) const
  {
  *camera->getWorldToCamera() = mathernogl::matrixTranslate(focalPosition * -1) * rotationMatrix * mathernogl::matrixTranslate(0, 0, -zoomOffset);
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

void TOInputHandler::onUpdate(GameContext* gameContext)
  {
  performMouseCameraMovement(gameContext);
  }

bool TOInputHandler::onKeyHeld(GameContext* gameContext, uint key)
  {
  TOGameContext* toGameContext = TOGameContext::cast(gameContext);
  char character = getCharFromKeyCode(key);
  Vector3D translation(0);
  float rotationDelta = 0;
  float speed = panSpeed * gameContext->getDeltaTime() * toGameContext->getSettings()->getCameraPanSpeed() * 0.001f;
  float rotateSpeed = yawSpeed * gameContext->getDeltaTime() * toGameContext->getSettings()->getCameraRotSpeed() * 0.001f;

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
      rotationDelta = -rotateSpeed;
      break;
    case 'E':
      rotationDelta = rotateSpeed;
      break;
    }

  if (translation.magnitude() > 0 || rotationDelta != 0)
    {
    if (rotationDelta != 0)
      {
      rotation += rotationDelta;
      refreshRotationMatrix();
      }
    translation *= mathernogl::matrixYaw(rotation);
    Vector3D newFocalPosition = focalPosition + translation;
    if (TOGameContext::cast(gameContext)->isPositionOnMap(newFocalPosition))
      {
      focalPosition = newFocalPosition;
      refreshCamera(gameContext->getCamera());
      }
    return true;
    }
  return false;
  }

void TOInputHandler::refreshRotationMatrix()
  {
  rotationMatrix = mathernogl::matrixYaw(-rotation) * mathernogl::matrixPitch(-pitch);
  }

bool TOInputHandler::onKeyPressed(GameContext* gameContext, uint key)
  {
  if (key == KEY_ESC)
    {
    TOGameContext::cast(gameContext)->displayPauseMenu();
    return true;
    }

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
  return false;
  }

bool TOInputHandler::onMouseHeld(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  if (button == MOUSE_LEFT)
    {
    TowerSelectionManager* selectionManager = TOGameContext::cast(gameContext)->getSelectionManager();
    if (!selectionManager->isMouseDragging())
      selectionManager->onStartMouseDrag(gameContext, mouseX, mouseY);
    else
      selectionManager->onUpdateMouseDrag(gameContext, mouseX, mouseY);
    return true;
    }
  return false;
  }

bool TOInputHandler::onMouseReleased(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  if (button == MOUSE_LEFT)
    {
    TowerSelectionManager* selectionManager = TOGameContext::cast(gameContext)->getSelectionManager();
    bool isCtrlDown = gameContext->getInputManager()->isKeyDown(KEY_LCTRL);
    if (selectionManager->isMouseDragging())
      {
      if(selectionManager->onFinishMouseDrag(gameContext, mouseX, mouseY, isCtrlDown))
        return true;
      }
    return selectionManager->onWorldClick(gameContext, mouseX, mouseY, isCtrlDown);
    }
  else if (button == MOUSE_RIGHT)
    {
    Vector3D worldPos = TOGameContext::cast(gameContext)->terrainHitTest(mouseX, mouseY);
    TOGameContext::cast(gameContext)->createBot(0, worldPos);
    return true;
    }
  return false;
  }

bool TOInputHandler::onMouseScroll(GameContext* gameContext, double scrollOffset, uint mouseX, uint mouseY)
  {
  zoomOffset -= scrollOffset * zoomSpeed;
  zoomOffset = mathernogl::clampf(zoomOffset, minZoom, maxZoom);
  refreshCamera(gameContext->getCamera());
  return true;
  }

void TOInputHandler::performMouseCameraMovement(GameContext* gameContext)
  {
  TOGameContext* toGameContext = TOGameContext::cast(gameContext);
  if (!toGameContext->getSettings()->enableScreenEdgePan())
    return;

  const uint edgeCamMoveWidth = 10;
  const Vector2D screenSize = gameContext->getRenderContext()->getWindow()->getSize();
  const Vector2D mousePos = gameContext->getInputManager()->getMousePos();

  Vector3D camTranslation;
  if (mousePos.x < edgeCamMoveWidth)
    camTranslation.x -= 1;
  else if (mousePos.x > (screenSize.x - edgeCamMoveWidth))
    camTranslation.x += 1;
  if (mousePos.y < edgeCamMoveWidth)
    camTranslation.z -= 1;
  else if (mousePos.y > (screenSize.y - edgeCamMoveWidth))
    camTranslation.z += 1;

  if (camTranslation.magnitude() > 0)
    {
    camTranslation.makeUniform();
    camTranslation *= mousePanSpeed * gameContext->getDeltaTime() * toGameContext->getSettings()->getCameraPanSpeed() * 0.001f;
    camTranslation *= mathernogl::matrixYaw(rotation);
    Vector3D newFocalPosition = focalPosition + camTranslation;
    if (TOGameContext::cast(gameContext)->isPositionOnMap(newFocalPosition))
      {
      focalPosition = newFocalPosition;
      refreshCamera(gameContext->getCamera());
      }
    }
  }
