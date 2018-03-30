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

bool TOInputHandler::onKeyHeld(GameContext* gameContext, uint key)
  {
  char character = getCharFromKeyCode(key);
  Vector3D translation(0);
  float rotationDelta = 0;
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
      rotationDelta = rotateSpeed;
      break;
    case 'E':
      rotationDelta = -rotateSpeed;
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
    focalPosition += translation;
    refreshCamera(gameContext->getCamera());
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
    gameContext->endContext();
    return false;
    }

  switch(getCharFromKeyCode(key))
    {
    case 'P':
      paused = !paused;
      gameContext->setPaused(paused);
      return true;
    case 'T':
      showSaveDialog(gameContext);
      return true;
    }
  return false;
  }

bool TOInputHandler::onMousePressed(GameContext* gameContext, uint button, uint mouseX, uint mouseY)
  {
  TOGameContext* toGameContext = TOGameContext::cast(gameContext);
  if (gameContext->getBoundingBoxManager()->boundingBoxPicked())
    {
    uint pickedTowerID = (uint)gameContext->getBoundingBoxManager()->getPickedBoundingBoxMeta();
    toGameContext->focusTower(pickedTowerID);
    return true;
    }

  toGameContext->unfocusTower();
  return false;
  }

bool TOInputHandler::onMouseScroll(GameContext* gameContext, double scrollOffset, uint mouseX, uint mouseY)
  {
  zoomOffset -= scrollOffset * zoomSpeed;
  zoomOffset = mathernogl::clampf(zoomOffset, minZoom, maxZoom);
  refreshCamera(gameContext->getCamera());
  return true;
  }

void TOInputHandler::showSaveDialog(GameContext* gameContext)
  {
  UIManager* uiManager = gameContext->getUIManager();
  std::shared_ptr<SaveLoadDlg> saveLoadDlg(new SaveLoadDlg(uiManager->getNextComponentID(), SaveLoadDlg::modeSave));
  uiManager->addComponent(saveLoadDlg);
  uiManager->enableModalMode(saveLoadDlg);

  //  On saving to a file
  uint id = saveLoadDlg->getID();
  saveLoadDlg->setSaveGameStateCallback([id, gameContext, uiManager]()
                                          {
                                          std::shared_ptr<TOGameState> state(new TOGameState());
                                          TOGameContext::cast(gameContext)->getGameState(state.get());
                                          uiManager->removeComponent(id);
                                          uiManager->disableModalMode();
                                          return state;
                                          });

  //  On cancelled loading
  saveLoadDlg->setCancelledCallback([uiManager, id]()
                                      {
                                      uiManager->removeComponent(id);
                                      uiManager->disableModalMode();
                                      });
  }
