#pragma once

#include <vector>
#include <mathernogl/Types.h>


struct HeightMap
  {
  std::vector<float> heights;
  uint width;
  };

class HeightMapFactory
  {
public:
  static void createFlatMap(HeightMap* heightMap, uint width, float height = 0);
  static void createNoiseMap(HeightMap* heightMap, uint width, float amplitude, float waveLength);
  static void createDiamondSquareMap(HeightMap* heightMap, uint numPasses, float initSpread, float spreadReductionFactor);
  };
