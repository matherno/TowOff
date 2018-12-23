//
// Created by matt on 23/12/18.
//

#include "VoxelStorage.h"


bool VoxelStorage::initialiseVAO()
  {
  numVoxels = 0;

  std::vector<float> bufferData;
  for (const mathernogl::VoxelPoint& voxel : voxels)
    {
    bufferData.emplace_back(voxel.x);
    bufferData.emplace_back(voxel.y);
    bufferData.emplace_back(voxel.z);
    bufferData.emplace_back(voxel.col);
    ++numVoxels;
    }

  mathernogl::clearGLErrors();
  mathernogl::GPUBufferStatic buffer;
  buffer.init();
  buffer.copyDataFloat(bufferData);
  vao.init();
  vao.bind();
  vao.linkBufferAsFloats(buffer, 4, 0, false);
  vao.unbind();
  buffer.cleanUp();
  ASSERT_NO_GL_ERROR();

  return glGetError() == GL_NO_ERROR;
  }

void VoxelStorage::cleanUp()
  {
  vao.cleanUp();
  numVoxels = 0;
  }
