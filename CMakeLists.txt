cmake_minimum_required(VERSION 3.7)
project(TowOff)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/RenderSystem/RenderContextImpl.cpp src/RenderSystem/RenderContextImpl.h src/RenderSystem/RenderableSetImpl.cpp src/RenderSystem/RenderableSetImpl.h src/RenderSystem/MeshStorage.cpp src/RenderSystem/MeshStorage.h src/RenderSystem/RenderableMesh.cpp src/RenderSystem/RenderableMesh.h src/RenderSystem/RenderSystem.h src/RenderSystem/TransformStack.cpp src/RenderSystem/TransformStack.h src/GameSystem/GameSystem.h src/GameSystem/GameContextImpl.cpp src/GameSystem/GameContextImpl.h src/GameSystem/InputCodes.h src/GameSystem/InputManagerImpl.cpp src/GameSystem/InputManagerImpl.h src/TestActor.cpp src/TestActor.h src/TestInputHandler.cpp src/TestInputHandler.h src/GameSystem/Camera.h src/PanCameraInputHandler.cpp src/PanCameraInputHandler.h src/Player.cpp src/Player.h src/Tower.cpp src/Tower.h src/TOGameContext.cpp src/TOGameContext.h src/InstantWeapon.cpp src/InstantWeapon.h src/ProjectileWeapon.cpp src/ProjectileWeapon.h src/Projectile.cpp src/Projectile.h src/Resources.h src/TimeToLiveActor.cpp src/TimeToLiveActor.h src/RenderSystem/RenderableTerrain.cpp src/RenderSystem/RenderableTerrain.h src/ParticleSystem/ParticleEmitter.cpp src/ParticleSystem/ParticleEmitter.h src/ParticleSystem/RenderableParticles.cpp src/ParticleSystem/RenderableParticles.h src/ParticleSystem/ParticleSystem.cpp src/ParticleSystem/ParticleSystem.h src/UISystem/UISystem.h src/UISystem/UIManagerImpl.cpp src/UISystem/UIManagerImpl.h src/UISystem/UIPanel.cpp src/UISystem/UIPanel.h src/UISystem/UIRenderable.cpp src/UISystem/UIRenderable.h)
add_executable(TowOff ${SOURCE_FILES})

include_directories(. ../MathernoGL/include)

target_link_libraries(TowOff /home/matt/SoftwareProjects/C++/MathernoGL/cmake-build-debug/libMathernoGL.a GL GLEW glfw freeimage)