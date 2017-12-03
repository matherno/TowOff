cmake_minimum_required(VERSION 3.7)
project(TowOff)

set(CMAKE_CXX_STANDARD 11)

set(SOURCE_FILES src/main.cpp src/RenderSystem/RenderContextImpl.cpp src/RenderSystem/RenderContextImpl.h src/RenderSystem/RenderableSetImpl.cpp src/RenderSystem/RenderableSetImpl.h src/RenderSystem/MeshStorage.cpp src/RenderSystem/MeshStorage.h src/RenderSystem/RenderableMesh.cpp src/RenderSystem/RenderableMesh.h src/RenderSystem/RenderSystem.h src/RenderSystem/TransformStack.cpp src/RenderSystem/TransformStack.h src/GameSystem/GameSystem.h src/GameSystem/GameContextImpl.cpp src/GameSystem/GameContextImpl.h src/GameSystem/InputCodes.h src/GameSystem/InputManagerImpl.cpp src/GameSystem/InputManagerImpl.h src/TestActor.cpp src/TestActor.h src/TestInputHandler.cpp src/TestInputHandler.h src/GameSystem/Camera.h src/PanCameraInputHandler.cpp src/PanCameraInputHandler.h)
add_executable(TowOff ${SOURCE_FILES})

include_directories(. ../MathernoGL/include)

target_link_libraries(TowOff /home/matt/SoftwareProjects/C++/MathernoGL/cmake-build-debug/libMathernoGL.a GL GLEW glfw freeimage)