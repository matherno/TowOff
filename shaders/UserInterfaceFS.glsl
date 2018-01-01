#version 330

uniform vec3 inColour = vec3(0, 0, 0.5);
uniform int inUseSolidColour = 1;
uniform sampler2D inTexture;

in vec2 texCoords;

out vec4 outputColour;

void main(){
    if (inUseSolidColour == 1)
	    outputColour = vec4(inColour, 1);
	else
	    outputColour = texture(inTexture, texCoords);
}
