//
// Created by matherno on 22/11/17.
//

#include <RenderSystem/RenderableMesh.h>
#include "TestActor.h"

TestActor::TestActor(uint id) : GameActor(id)
  {

  }

void TestActor::onAttached(GameContext* gameContext)
  {
  RenderContext* context = gameContext->getRenderContext();
  MeshStoragePtr meshStorage = context->createMeshStorage("resources/TropTree1.obj");
  if(meshStorage)
    {
    RenderableMesh* renderable = new RenderableMesh(context->getNextRenderableID());
    renderable->setMeshStorage(MeshStoragePtr(meshStorage));
    renderable->initialise(context);
    renderable->getTransform()->translate(8, 0, 0);
    context->getRenderableSet()->addRenderable(RenderablePtr(renderable));
    }

  MeshStoragePtr meshStorage2 = context->createMeshStorage("resources/TropTree2.obj");
  if(meshStorage2)
    {
    RenderableMesh* renderable = new RenderableMesh(context->getNextRenderableID());
    renderable->setMeshStorage(MeshStoragePtr(meshStorage2));
    renderable->initialise(context);
    renderable->getTransform()->translate(-8, 0, 0);
    context->getRenderableSet()->addRenderable(RenderablePtr(renderable));
    }

  MeshStoragePtr meshStorage3 = context->createMeshStorage("resources/TropTree3.obj");
  if(meshStorage3)
    {
    RenderableMesh* renderable = new RenderableMesh(context->getNextRenderableID());
    renderable->setMeshStorage(MeshStoragePtr(meshStorage3));
    renderable->initialise(context);
    tree.reset(renderable);
    context->getRenderableSet()->addRenderable(tree);
    }
  }

void TestActor::onUpdate(GameContext* gameContext)
  {
  }

void TestActor::onDetached(GameContext* gameContext)
  {

  }

void TestActor::translate(Vector3D translation)
  {
  tree->getTransform()->translate(translation);
  }
