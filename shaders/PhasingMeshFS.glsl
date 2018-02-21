#version 330

in float height;

out vec4 outputColour;

uniform vec3 inColour;
uniform float inAlpha = 1;
uniform int inTimeMS;
uniform float inPhaseSpeed = 0.005;

void main(){
  float phaseFactor = sin(height * 2 + inTimeMS * inPhaseSpeed) * 0.05 + 0.05;
  outputColour = mix(vec4(inColour, inAlpha), vec4(1, 1, 1, 1), phaseFactor);
}
