//
// Created by matherno on 17/11/17.
//

#include "RenderableSetImpl.h"

RenderableSetImpl::RenderableSetImpl(uint id) : RenderableSet(id)
  {
  }

void RenderableSetImpl::initialise(RenderContext* renderContext)
  {
  }

void RenderableSetImpl::cleanUp(RenderContext* renderContext)
  {
  }

void RenderableSetImpl::render(RenderContext* renderContext)
  {
  const Vector4D* clipPlane = getClippingPlane();
  bool usingParentClipPlane = clipPlane->x != 0 || clipPlane->y != 0 || clipPlane->z != 0;

  for (RenderablePtr renderable : *renderables.getList())
    {
    if (!usingParentClipPlane)
      renderContext->setClippingPlane(*renderable->getClippingPlane());

    renderContext->pushTransform(renderable->getTransform());
    renderable->render(renderContext);
    renderContext->popTransform();

    if (!usingParentClipPlane)
      renderContext->disableClippingPlane();
    }
  }

void RenderableSetImpl::addRenderable(RenderablePtr renderable)
  {
  if (!containsRenderable(renderable->getID()))
    {
    renderables.add(renderable, renderable->getID());
    }
  }

void RenderableSetImpl::removeRenderable(int id)
  {
  renderables.remove(id);
  }

bool RenderableSetImpl::containsRenderable(int id) const
  {
  return renderables.contains(id);
  }

int RenderableSetImpl::count() const
  {
  return renderables.count();
  }

void RenderableSetImpl::forEachChild(std::function<void(RenderablePtr)> function)
  {
  for (RenderablePtr& child : *renderables.getList())
    function(child);
  }
