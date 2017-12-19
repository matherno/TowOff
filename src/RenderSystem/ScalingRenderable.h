#pragma once

#include <src/RenderSystem/RenderSystem.h>

/*
*   Will crudely scale it's wrapped renderable with each render, by the given scaleRate
*/

class ScalingRenderable : public Renderable
  {
private:
  RenderablePtr renderable;
  float scaleRate;

public:
  ScalingRenderable(uint id, RenderablePtr renderable, float scaleRate);

  virtual void initialise(RenderContext* renderContext) override;
  virtual void cleanUp(RenderContext* renderContext) override;
  virtual void render(RenderContext* renderContext) override;
  virtual uint getID() const override;
  virtual Transform* getTransform() override;
  virtual void setTransform(const Transform& transform) override;
  };
