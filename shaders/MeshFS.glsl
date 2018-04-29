#version 330

#define DRAW_STYLE_SINGLE_COLOUR  1
#define DRAW_STYLE_TEXTURE        2
#define DRAW_STYLE_VERT_COLOUR    3

in vec3 normal;
in vec3 colour;
in vec2 texCoords;

out vec4 outputColour;

uniform int inLightShaded = 1;
uniform float inAlpha = 1;
uniform sampler2D inTexture;
uniform int inDrawStyle = DRAW_STYLE_SINGLE_COLOUR;

vec3 getColour(){
  if (inDrawStyle == DRAW_STYLE_TEXTURE)
    return texture(inTexture, texCoords).rgb;
  else
    return colour;
}

void main(){
    float lightFactor = 1;
    if (inLightShaded > 0){
        lightFactor = dot(normal, normalize(vec3(0.5, 0.2, 0.3)));
        lightFactor *= 0.5;
        lightFactor += 0.5;
    }
	outputColour = vec4(getColour() * max(lightFactor, 0), inAlpha);
}
