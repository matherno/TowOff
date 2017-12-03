#version 330

in vec3 normal;

out vec4 outputColour;

void main(){
    float lightFactor = dot(normal, normalize(vec3(0.5, 0.5, 0.5)));
    lightFactor *= 0.5;
    lightFactor += 0.6;
	outputColour = vec4(0.2, 0.5, 0.6, 1) * max(lightFactor, 0);
}
