//
// Created by matherno on 17/11/17.
//

#include "RenderContextImpl.h"
#include "RenderableSetImpl.h"

using namespace mathernogl;

bool RenderContextImpl::initialise(const RenderInitConfig* initConfig)
  {
  clearGLErrors();
  window.reset(initGL(initConfig->windowName, initConfig->windowWidth, initConfig->windowHeight, initConfig->fullscreen, initConfig->antiAliasing));
  if (window)
    {
    window->setClearColour(0, 0, 0);
    renderableSet.reset(new RenderableSetImpl(getNextRenderableID()));
    ASSERT_NO_GL_ERROR();
    return true;
    }
  return false;
  }

bool RenderContextImpl::cleanUp()
  {
  //todo: cleanup all opengl objects
  return true;
  }

uint RenderContextImpl::getNextRenderableID()
  {
  return nextRenderableID++;
  }

void RenderContextImpl::setWorldToCamera(const Matrix4& transform)
  {
  worldToCameraTransform = transform;
  }

void RenderContextImpl::setCameraToClip(const Matrix4& transform)
  {
  cameraToClipTransform = transform;
  }

void RenderContextImpl::render()
  {
  if (!isRendering)
    {
    isRendering = true;
    transformStack.clear();
    window->clear();
    pushTransform(renderableSet->getTransform());
    renderableSet->render(this);
    popTransform();
    window->update();
    isRendering = false;
    }
  }

bool RenderContextImpl::isWindowOpen() const
  {
  return window->isOpen();
  }

void RenderContextImpl::activateShaderProgram(ShaderProgramPtr shaderProgram)
  {
  //todo: keep track of enabled shader and don't enable this if already enabled
  shaderProgram->enable();

  if (shaderProgram->hasUniformVariable(SHADER_VAR_WORLD_TO_CAMERA))
    shaderProgram->setVarMat4(SHADER_VAR_WORLD_TO_CAMERA, worldToCameraTransform);
  if (shaderProgram->hasUniformVariable(SHADER_VAR_CAMERA_TO_CLIP))
    shaderProgram->setVarMat4(SHADER_VAR_CAMERA_TO_CLIP, cameraToClipTransform);
  if (shaderProgram->hasUniformVariable(SHADER_VAR_VERT_TO_WORLD))
    {
    if (getStackedTransform())
      shaderProgram->setVarMat4(SHADER_VAR_VERT_TO_WORLD, *getStackedTransform()->getTransformMatrix());
    else
      shaderProgram->setVarMat4(SHADER_VAR_VERT_TO_WORLD, Matrix4(1));
    }
  if (shaderProgram->hasUniformVariable(SHADER_VAR_SCREEN_SIZE))
    shaderProgram->setVarVec2(SHADER_VAR_SCREEN_SIZE, getWindow()->getSize());

  //todo: put vert to world shader variable set in a more appropriate place

  }

ShaderProgramPtr RenderContextImpl::createShaderProgram(const std::vector<Shader>* shaders)
  {
  //todo: cache all created shader programs, and ensure that duplicates aren't created
  ShaderProgram* shaderProgram = new ShaderProgram();
  shaderProgram->init(*shaders);
  return ShaderProgramPtr(shaderProgram);
  }

MeshStoragePtr RenderContextImpl::createMeshStorage(const std::string& objFilePath)
  {
  //todo: cache the mesh storages, ensuring no duplicates are loaded into memory
  MeshStoragePtr meshStorage(new MeshStorage(nextMeshStorageID++));
  loadObj(objFilePath, &meshStorage->indices, &meshStorage->vertices, &meshStorage->normals, &meshStorage->texCoords);
  if(meshStorage->initialiseVAO())
    return meshStorage;
  else
    return nullptr;
  }

MeshStoragePtr RenderContextImpl::createEmptyMeshStorage()
  {
  return std::make_shared<MeshStorage>(nextMeshStorageID++);
  }

void RenderContextImpl::pushTransform(const mathernogl::Transform* transform)
  {
  transformStack.push(transform);
  }

void RenderContextImpl::popTransform()
  {
  transformStack.pop();
  }

const Transform* RenderContextImpl::getStackedTransform()
  {
  return transformStack.getTop();
  }

Window* RenderContextImpl::getWindow()
  {
  return window.get();
  }

const Window* RenderContextImpl::getWindow() const
  {
  return window.get();
  }

const Matrix4* RenderContextImpl::getWorldToCamera() const
  {
  return &worldToCameraTransform;
  }

const Matrix4* RenderContextImpl::getCameraToClip() const
  {
  return &cameraToClipTransform;
  }

TexturePtr RenderContextImpl::createTexture(const string& imageFilePath)
  {
  //todo: cache these
  return TexturePtr(createTextureFromFile(imageFilePath, false));
  }

uint RenderContextImpl::bindTexture(TexturePtr texture)
  {
  //todo: cache what is bound, and not binding if already bound to a location
  uint glBindLocation = 0;
  if (texIDsToBoundLocals.find(texture->glTexID) != texIDsToBoundLocals.end())
    {
    glBindLocation = texIDsToBoundLocals[texture->glTexID];
    }
  else
    {
    glBindLocation = nextTexBoundLocal++;
    glActiveTexture(GL_TEXTURE0 + glBindLocation);
    glBindTexture(texture->glTexType, texture->glTexID);
    texIDsToBoundLocals[texture->glTexID] = glBindLocation;
    }
  return glBindLocation;
  }

