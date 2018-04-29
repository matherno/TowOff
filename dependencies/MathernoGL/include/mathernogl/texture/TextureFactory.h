#pragma once

#include <stdexcept>
#include <GL/glew.h>

#include <mathernogl/utils/Logging.h>
#include <mathernogl/utils/ImageImporter.h>
#include <mathernogl/image/ImageData.h>
#include <mathernogl/texture/Texture.h>

namespace mathernogl {

enum TextureFiltering {
	NEAREST, LINEAR
};

enum TextureWrapping {
	REPEAT, REPEAT_MIRRORED, CLAMP_EDGE, CLAMP_BORDER
};

struct TextureOptions {
  bool genMipMaps = false;
  TextureFiltering filtering = NEAREST;
  TextureWrapping wrapping = REPEAT;
  bool gammaCorrect = true;
};

Texture* createTextureFromFile(const std::string& filePath, const TextureOptions& options = TextureOptions());
Texture* createEmptyTexture(uint width, uint height, uint bytesPerPixel = 3, const TextureOptions& options = TextureOptions());
Texture* createEmptyAlphaTexture(uint width, uint height, const TextureOptions& options = TextureOptions());
void updateTexture(Texture* texture);

/*
 * Creates a texture within opengl, returning the ID handle to it
 * if not RGB (rgbFormat), then will read the texture bytes in BGR format
 */
uint createGLTexture(uint glTexType, uint width, uint height, uint bytesPerPixel, const TextureOptions& options, bool rgbFormat, const byte* byteData);

}
