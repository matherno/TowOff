//
// Created by matt on 10/12/17.
//

#include "ScalingRenderable.h"

ScalingRenderable::ScalingRenderable(uint id, RenderablePtr renderable, float scaleRate) : Renderable(id), renderable(renderable), scaleRate(scaleRate)
  {}

void ScalingRenderable::initialise(RenderContext* renderContext)
  {
  renderable->initialise(renderContext);
  }

void ScalingRenderable::cleanUp(RenderContext* renderContext)
  {
  renderable->cleanUp(renderContext);
  }

void ScalingRenderable::render(RenderContext* renderContext)
  {
  Vector3D position = renderable->getTransform()->getTranslationComponent();
  renderable->getTransform()->translate(position.inverse());
  renderable->getTransform()->scale(scaleRate);
  renderable->getTransform()->translate(position);
  renderable->render(renderContext);
  }

uint ScalingRenderable::getID() const
  {
  return renderable->getID();
  }

Transform* ScalingRenderable::getTransform()
  {
  return renderable->getTransform();
  }

void ScalingRenderable::setTransform(const Transform& transform)
  {
  renderable->setTransform(transform);
  }
