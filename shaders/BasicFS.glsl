#version 330

in vec2 colour;

out vec4 outputColour;

void main(){
	outputColour = vec4(colour, 0, 1);
}
