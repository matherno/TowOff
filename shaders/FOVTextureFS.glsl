#version 330

uniform sampler2D inTexture;
uniform sampler2D inVisibilityTexture;
uniform float inVisibilityWidth = 1;
uniform float inVisibilityHeight = 1;
uniform vec3 inFogColour = vec3(0.005);

in vec2 worldPos;    // x,z

out vec4 outputColour;

void main()
  {
	float alpha = 1.0;
	float halfWidth = inVisibilityWidth * 0.5;
  if (worldPos.x >= -1 * halfWidth && worldPos.y >= -1 * halfWidth && worldPos.x <= halfWidth && worldPos.y <= halfWidth)
    {
    vec2 textureCoord = worldPos;
    textureCoord.y *= -1;
    textureCoord = (textureCoord + halfWidth) / vec2(inVisibilityWidth, inVisibilityHeight);
    alpha = 1.0 - texture(inVisibilityTexture, textureCoord).r;
    }
	outputColour = vec4(inFogColour, alpha);
  }
