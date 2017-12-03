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
  for (RenderablePtr renderable : *renderables.getList())
    {
    renderContext->pushTransform(renderable->getTransform());
    renderable->render(renderContext);
    renderContext->popTransform();
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
