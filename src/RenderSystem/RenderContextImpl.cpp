//
// Created by matherno on 17/11/17.
//

#include "RenderContextImpl.h"
#include "RenderableSetImpl.h"

using namespace mathernogl;

bool RenderContextImpl::initialise(const RenderInitConfig* initConfig)
  {
  nextTexBoundLocal = 1;
  nextMeshStorageID = 1;
  nextRenderableID = 1;

  clearGLErrors();
  window.reset(initGL(initConfig->windowName, initConfig->windowWidth, initConfig->windowHeight, initConfig->fullscreen, initConfig->antiAliasing));
  if (window)
    {
    window->setClearColour(0, 0, 0);
    renderableSet.reset(new RenderableSetImpl(getNextRenderableID()));
    registerStandardDrawStages();
    glGetIntegerv(GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS, &maxNumTextureLocations);
    ASSERT_NO_GL_ERROR();
    return true;
    }
  return false;
  }

bool RenderContextImpl::cleanUp()
  {
  reset();
  window->close();
  return true;
  }

void RenderContextImpl::reset()
  {
  resourceCache.forEachShaderProgram([](ShaderProgramPtr shaderProgram)
    {
    shaderProgram->cleanUp();
    });
  resourceCache.forEachMeshStorage([](MeshStoragePtr meshStorage)
    {
    meshStorage->cleanUp();
    });
  resourceCache.forEachTexture([](TexturePtr texture)
    {
    texture->cleanUp();
    });
  resourceCache.clearAll();
  texIDsToBoundLocals.clear();
  drawStages.clear();
  registerStandardDrawStages();
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
    window->clear();
    boundShaderID = 0;
    for (int stage : drawStages)
      {
      activeDrawStage = stage;
      transformStack.clear();
      pushTransform(renderableSet->getTransform());
      setClippingPlane(*renderableSet->getClippingPlane());
      renderableSet->render(this);
      disableClippingPlane();
      popTransform();
      }
    window->update();
    activeDrawStage = DRAW_STAGE_NONE;
    isRendering = false;
    }
  }

bool RenderContextImpl::isWindowOpen() const
  {
  return window->isOpen();
  }

void RenderContextImpl::activateShaderProgram(ShaderProgramPtr shaderProgram)
  {
  if (shaderProgram->getGLID() != boundShaderID)
    {
    shaderProgram->enable();
    boundShaderID = shaderProgram->getGLID();
    }

  //todo: keep state of whether or not each shader program has already been given these variables that frame (not for clipping uniforms)

  //  transform matrices
  shaderProgram->setVarMat4(SHADER_VAR_WORLD_TO_CAMERA, worldToCameraTransform, true);
  shaderProgram->setVarMat4(SHADER_VAR_CAMERA_TO_CLIP, cameraToClipTransform, true);
  if (getStackedTransform())
    shaderProgram->setVarMat4(SHADER_VAR_VERT_TO_WORLD, *getStackedTransform()->getTransformMatrix(), true);
  else
    shaderProgram->setVarMat4(SHADER_VAR_VERT_TO_WORLD, Matrix4(1), true);

  //  clipping
  bool enableClipping = clipPlane.x != 0 || clipPlane.y != 0 || clipPlane.z != 0;
  shaderProgram->setVarInt(SHADER_VAR_CLIPPING_ENABLED, enableClipping ? 1 : 0, true);
  if (enableClipping)
    shaderProgram->setVarVec4(SHADER_VAR_CLIP_PLANE, clipPlane, true);

  //  misc
  shaderProgram->setVarVec2(SHADER_VAR_SCREEN_SIZE, getWindow()->getSize(), true);
  shaderProgram->setVarInt(SHADER_VAR_TIME_MS, (int)(getTimeMS() - startTime), true);

  }

void logCreateShaderProgram(ShaderProgramPtr shaderProgram, const std::vector<mathernogl::Shader>* shaders)
  {
  string msg = "Created shader program: " + std::to_string(shaderProgram->getGLID());
  for (const mathernogl::Shader& shader : *shaders)
    msg += ", '" + shader.filePath + "'";
  logInfo(msg);
  }

ShaderProgramPtr RenderContextImpl::getSharedShaderProgram(const std::vector<mathernogl::Shader>* shaders)
  {
  if (ShaderProgramPtr cachedShaderProgram = resourceCache.getShaderProgram(shaders))
    return cachedShaderProgram;

  ShaderProgramPtr shaderProgram (new ShaderProgram());
  shaderProgram->init(*shaders);
  resourceCache.addShaderProgram(shaderProgram, shaders);
  logCreateShaderProgram(shaderProgram, shaders);
  return shaderProgram;
  }
MeshStoragePtr RenderContextImpl::getSharedMeshStorage(const std::string& objFilePath)
  {
  if (MeshStoragePtr cachedMeshStorage = resourceCache.getMeshStorage(objFilePath))
    return cachedMeshStorage;

  MeshStoragePtr meshStorage(new MeshStorage(nextMeshStorageID++));
  loadObj(objFilePath, &meshStorage->indices, &meshStorage->vertices, &meshStorage->normals, &meshStorage->texCoords);
  meshStorage->calculateMinMax();
  if(meshStorage->initialiseVAO())
    {
    resourceCache.addMeshStorage(meshStorage, objFilePath);
    logInfo("Created mesh storage: " + std::to_string(meshStorage->getID()) + ", '" + objFilePath + "'");
    return meshStorage;
    }
  else
    {
    logWarning("Failed to create mesh storage!: " + std::to_string(meshStorage->getID()) + ", '" + objFilePath + "'");
    return nullptr;
    }
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

TexturePtr RenderContextImpl::getSharedTexture(const string& imageFilePath, TextureOptions options)
  {
  if (TexturePtr texture = resourceCache.getTexture(imageFilePath))
    return texture;

  TexturePtr texture = TexturePtr(createTextureFromFile(imageFilePath, options.generateMipMaps, options.filtering, options.wrapping));
  resourceCache.addTexture(texture, imageFilePath);
  logInfo("Created texture: " + std::to_string(texture->glTexID) + ", '" + imageFilePath + "'");
  return texture;
  }

TexturePtr RenderContextImpl::createEmptyTexture(uint width, uint height, uint bytesPerPixel, TextureOptions options)
  {
  return TexturePtr(mathernogl::createEmptyTexture(width, height, options.filtering, options.wrapping, bytesPerPixel));
  }

FontPtr RenderContextImpl::getSharedFont(const string& fntFilePath, const string& glyphsFilePath)
  {
  TexturePtr glyphsTexture = getSharedTexture(glyphsFilePath);
  FontDefinitionPtr fontDefinition = resourceCache.getFontDefinition(fntFilePath);
  if (!fontDefinition)
    {
    fontDefinition = readFontFile(fntFilePath);
    resourceCache.addFontDefinition(fontDefinition, fntFilePath);
    logInfo("Created font: '" + fntFilePath + "'");
    }
  return FontPtr(new Font(fontDefinition, glyphsTexture));
  }


uint RenderContextImpl::bindTexture(TexturePtr texture)
  {
  uint glBindLocation = 0;
  if (texIDsToBoundLocals.find(texture->glTexID) != texIDsToBoundLocals.end())
    {
    glBindLocation = texIDsToBoundLocals[texture->glTexID];
    }
  else
    {
    if (nextTexBoundLocal >= maxNumTextureLocations)
      {
      logWarning("Ran out of texture bound locations! Max: " + std::to_string(maxNumTextureLocations));
      nextTexBoundLocal = 1;
      }

    //  nextTexBoundLocal is initialised to 1, and active texture reset back to 0 after binding,
    //      then external texture bindings don't mess things up
    glBindLocation = nextTexBoundLocal++;
    glActiveTexture(GL_TEXTURE0 + glBindLocation);
    glBindTexture(texture->glTexType, texture->glTexID);
    texIDsToBoundLocals[texture->glTexID] = glBindLocation;
    glActiveTexture(GL_TEXTURE0);
    }
  return glBindLocation;
  }

RenderableSetPtr RenderContextImpl::createRenderableSet()
  {
  return RenderableSetPtr(new RenderableSetImpl(getNextRenderableID()));
  }

void RenderContextImpl::setClippingPlane(const Vector4D& plane)
  {
  bool clippingEnabled = isClippingEnabled();
  if (plane.x != 0 || plane.y != 0 || plane.z != 0)
    {
    if (!clippingEnabled)
      glEnable(GL_CLIP_DISTANCE0);
    clipPlane = plane;
    }
  }

void RenderContextImpl::disableClippingPlane()
  {
  if (isClippingEnabled())
    {
    glDisable(GL_CLIP_DISTANCE0);
    clipPlane.set(0, 0, 0, 0);
    }
  }

bool RenderContextImpl::isClippingEnabled()
  {
  return (clipPlane.x != 0 || clipPlane.y != 0 || clipPlane.z != 0);
  }

bool RenderContextImpl::registerDrawStage(int drawStage)
  {
  if (drawStages.count(drawStage) == 0)
    {
    drawStages.insert(drawStage);
    return true;
    }
  return false;
  }

void RenderContextImpl::registerStandardDrawStages()
  {
  registerDrawStage(DRAW_STAGE_OPAQUE);
  registerDrawStage(DRAW_STAGE_TRANSPARENT);
  registerDrawStage(DRAW_STAGE_OVERLAY);
  registerDrawStage(DRAW_STAGE_UI);
  }
