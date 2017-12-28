#version 330

uniform vec3 inColour = vec3(0, 0, 0.5);

out vec4 outputColour;

void main(){
	outputColour = vec4(inColour, 1);
}
