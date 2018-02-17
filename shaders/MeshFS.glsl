#version 330

in vec3 normal;
in vec3 colour;

out vec4 outputColour;

uniform int inLightShaded = 1;
uniform float inAlpha = 1;

void main(){
    float lightFactor = 1;
    if (inLightShaded > 0){
        lightFactor = dot(normal, normalize(vec3(0.5, 0.2, 0.3)));
        lightFactor *= 0.5;
        lightFactor += 0.6;
    }
	outputColour = vec4(colour * max(lightFactor, 0), inAlpha);
}
