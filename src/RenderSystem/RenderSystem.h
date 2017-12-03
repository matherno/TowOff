#pragma once
//
// Created by matherno on 20/11/17.
//

#include <mathernogl/MathernoGL.h>
#include "MeshStorage.h"

typedef mathernogl::Matrix4 Matrix4;
typedef mathernogl::Vector4D Vector4D;
typedef mathernogl::Vector3D Vector3D;
typedef mathernogl::Vector2D Vector2D;
typedef mathernogl::Transform Transform;
typedef std::string string;

/*
*   Renderable, represents an object or primitive that may be rendered
*/
class RenderContext;
class Renderable {
private:
  const uint id;
  mathernogl::Transform transform;

public:
  Renderable(uint id) : id(id) {}
  virtual ~Renderable() {}
  uint getID() const { return id; }
  virtual void initialise(RenderContext* renderContext) = 0;
  virtual void cleanUp(RenderContext* renderContext) = 0;
  virtual void render(RenderContext* renderContext) = 0;
  virtual Transform* getTransform() { return &transform; }
  virtual void setTransform(const Transform& transform) { this->transform = transform; }
  };
typedef std::shared_ptr<Renderable> RenderablePtr;

/*
*   Renderable set, containing a collection of renderables, which will all be rendered on a call to render.
*   Does not take ownership of renderables.
*/
class RenderableSet : public Renderable
  {
public:
  RenderableSet(uint id) : Renderable(id) {}
  virtual ~RenderableSet() {}
  virtual void addRenderable(RenderablePtr renderable) = 0;
  virtual void removeRenderable(int id) = 0;
  virtual bool containsRenderable(int id) const = 0;
  virtual int count() const = 0;
  };
typedef std::shared_ptr<RenderableSet> RenderableSetPtr;

/*
*   RenderInitConfig, simple struct to pass the initial config parameters to the render context
*/
struct RenderInitConfig
  {
  uint windowWidth = 800;
  uint windowHeight = 600;
  string windowName = "";
  bool fullscreen = false;
  bool antiAliasing = false;
  };

/*
*   RenderContext, main rendering manager
*/
typedef std::shared_ptr<mathernogl::ShaderProgram> ShaderProgramPtr;
class RenderContext
  {
public:
  RenderContext() {}
  virtual ~RenderContext() {}
  virtual bool initialise(const RenderInitConfig* initConfig) = 0;
  virtual bool cleanUp() = 0;
  virtual uint getNextRenderableID() = 0;
  virtual RenderableSetPtr getRenderableSet() = 0;
  virtual void setWorldToCamera(const Matrix4& transform) = 0;
  virtual void setCameraToClip(const Matrix4& transform) = 0;
  virtual void render() = 0;
  virtual bool isWindowOpen() const = 0;
  virtual void activateShaderProgram(ShaderProgramPtr shaderProgram) = 0;
  virtual ShaderProgramPtr createShaderProgram(const std::vector<mathernogl::Shader>* shaders) = 0;
  virtual MeshStoragePtr createMeshStorage(const string& objFilePath) = 0;
  virtual void pushTransform(const Transform* transform) = 0;
  virtual void popTransform() = 0;
  virtual const Transform* getStackedTransform() = 0;
  virtual mathernogl::Window* getWindow() = 0;
  };
typedef std::shared_ptr<RenderContext> RenderContextPtr;
