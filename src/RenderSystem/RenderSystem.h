#pragma once
//
// Created by matherno on 20/11/17.
//

#include <mathernogl/MathernoGL.h>
#include "MeshStorage.h"
#include "Font.h"

typedef mathernogl::Matrix4 Matrix4;
typedef mathernogl::Vector4D Vector4D;
typedef mathernogl::Vector3D Vector3D;
typedef mathernogl::Vector2D Vector2D;
typedef mathernogl::Transform Transform;
typedef std::string string;

/*
 *  Rendering is done in stages, each renderable has a draw stage that its render() function gets called in
 *  Stages are performed in numerical order
 *  Draw stage "DRAW_STAGE_ALL" means that the given renderable will have its render() function called in every stage
 */
#define DRAW_STAGE_NONE           -1
#define DRAW_STAGE_ALL             0
#define DRAW_STAGE_OPAQUE         10
#define DRAW_STAGE_TRANSPARENT    20
#define DRAW_STAGE_OVERLAY        30
#define DRAW_STAGE_UI             40

/*
*   Renderable, represents an object or primitive that may be rendered
*/
class RenderContext;
class Renderable {
private:
  const uint id;
  const int drawStage;
  mathernogl::Transform transform;
  Vector4D clipPlane = Vector4D(0);

public:
  Renderable(uint id, int drawStage = DRAW_STAGE_OPAQUE) : id(id), drawStage(drawStage) {}
  virtual ~Renderable() {}

  virtual uint getID() const { return id; }
  virtual void initialise(RenderContext* renderContext) = 0;
  virtual void cleanUp(RenderContext* renderContext) = 0;
  virtual void render(RenderContext* renderContext) = 0;
  virtual Transform* getTransform() { return &transform; }
  virtual const Transform* getTransform() const { return &transform; }
  virtual const Vector4D* getClippingPlane() const { return &clipPlane; }
  virtual void setTransform(const Transform& transform) { this->transform = transform; }
  virtual void setClippingPlane(const Vector4D& plane) { this->clipPlane = plane; }
  virtual int getDrawStage() const { return drawStage; }
  };
typedef std::shared_ptr<Renderable> RenderablePtr;

/*
*   Renderable set, containing a collection of renderables, which will all be rendered on a call to render.
*   Does not take ownership of renderables.
*/
class RenderableSet : public Renderable
  {
public:
  RenderableSet(uint id) : Renderable(id, DRAW_STAGE_ALL) {}
  virtual ~RenderableSet() {}
  virtual void addRenderable(RenderablePtr renderable) = 0;
  virtual void removeRenderable(int id) = 0;
  virtual bool containsRenderable(int id) const = 0;
  virtual void forEachChild(std::function<void(RenderablePtr child)>) = 0;
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

struct TextureOptions
  {
  mathernogl::TextureFiltering filtering = mathernogl::LINEAR;
  mathernogl::TextureWrapping wrapping = mathernogl::REPEAT;
  bool generateMipMaps = false;
  };

/*
*   RenderContext, main rendering manager
*/
typedef std::shared_ptr<mathernogl::ShaderProgram> ShaderProgramPtr;
typedef std::shared_ptr<mathernogl::Texture> TexturePtr;
class RenderContext
  {
public:
  RenderContext() {}
  virtual ~RenderContext() {}
  virtual bool initialise(const RenderInitConfig* initConfig) = 0;
  virtual bool cleanUp() = 0;
  virtual void reset() = 0;     //  should clear all caches and stored state, but leave the window open
  virtual uint getNextRenderableID() = 0;
  virtual RenderableSetPtr getRenderableSet() = 0;
  virtual void setWorldToCamera(const Matrix4& transform) = 0;
  virtual void setCameraToClip(const Matrix4& transform) = 0;
  virtual const Matrix4* getWorldToCamera() const = 0;
  virtual const Matrix4* getCameraToClip() const = 0;
  virtual void render() = 0;
  virtual bool isWindowOpen() const = 0;
  virtual void activateShaderProgram(ShaderProgramPtr shaderProgram) = 0;
  virtual uint bindTexture(TexturePtr texture) = 0;
  virtual ShaderProgramPtr getSharedShaderProgram(const std::vector<mathernogl::Shader>* shaders) = 0;
  virtual MeshStoragePtr getSharedMeshStorage(const string& objFilePath) = 0;
  virtual MeshStoragePtr createEmptyMeshStorage() = 0;
  virtual TexturePtr getSharedTexture(const string& imageFilePath, TextureOptions options = TextureOptions()) = 0;
  virtual TexturePtr createEmptyTexture(uint width, uint height, uint bytesPerPixel, TextureOptions options = TextureOptions()) = 0;
  virtual RenderableSetPtr createRenderableSet() = 0;
  virtual FontPtr getSharedFont(const string& fntFilePath, const string& glyphsFilePath) = 0;
  virtual void pushTransform(const Transform* transform) = 0;
  virtual void popTransform() = 0;
  virtual const Transform* getStackedTransform() = 0;
  virtual mathernogl::Window* getWindow() = 0;
  virtual const mathernogl::Window* getWindow() const = 0;
  virtual void setClippingPlane(const Vector4D& plane) = 0;
  virtual void disableClippingPlane() = 0;
  virtual bool isClippingEnabled() = 0;
  virtual bool registerDrawStage(int drawStage) = 0;    // returns false if the stage already exists
  virtual int getActiveDrawStage() const = 0;
  };
typedef std::shared_ptr<RenderContext> RenderContextPtr;
