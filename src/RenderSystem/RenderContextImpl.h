#pragma once
//
// Created by matherno on 17/11/17.
//

#include "RenderSystem.h"
#include "TransformStack.h"

#define SHADER_VAR_VERT_TO_WORLD "inVertToWorld"
#define SHADER_VAR_WORLD_TO_CAMERA "inWorldToCamera"
#define SHADER_VAR_CAMERA_TO_CLIP "inCameraToClip"
#define SHADER_VAR_SCREEN_SIZE "inScreenSize"

class RenderContextImpl : public RenderContext
  {
private:
  std::unique_ptr<mathernogl::Window> window;
  uint nextRenderableID = 1;
  uint nextMeshStorageID = 1;
  bool isRendering = false;
  RenderableSetPtr renderableSet;
  Matrix4 worldToCameraTransform;
  Matrix4 cameraToClipTransform;
  TransformStack transformStack;
  std::map<uint, uint> texIDsToBoundLocals;
  uint nextTexBoundLocal = 0;

public:
  virtual bool initialise(const RenderInitConfig* initConfig) override;
  virtual bool cleanUp() override;
  virtual mathernogl::Window* getWindow() override;
  virtual const mathernogl::Window* getWindow() const override;

  virtual uint getNextRenderableID() override;
  virtual RenderableSetPtr getRenderableSet() override { return renderableSet; }

  virtual void setWorldToCamera(const Matrix4& transform) override;
  virtual void setCameraToClip(const Matrix4& transform) override;
  virtual const Matrix4* getWorldToCamera() const override;
  virtual const Matrix4* getCameraToClip() const override;
  virtual void render() override;
  virtual bool isWindowOpen() const override;

  virtual void activateShaderProgram(ShaderProgramPtr shaderProgram) override;
  virtual uint bindTexture(TexturePtr texture) override;
  virtual ShaderProgramPtr createShaderProgram(const std::vector<mathernogl::Shader>* shaders) override;
  virtual MeshStoragePtr createMeshStorage(const std::string& objFilePath) override;
  virtual MeshStoragePtr createEmptyMeshStorage() override;
  virtual TexturePtr createTexture(const string& imageFilePath) override;

  virtual void pushTransform(const mathernogl::Transform* transform) override;
  virtual void popTransform() override;
  virtual const mathernogl::Transform* getStackedTransform() override;
  };
