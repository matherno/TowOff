#version 330

layout(location = 0) in vec4 inVert;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec3 inVertColour;

uniform mat4 inVertToWorld = mat4(1);
uniform mat4 inWorldToCamera = mat4(1);
uniform mat4 inCameraToClip = mat4(1);
uniform vec3 inColour;
uniform int inUseSingleColour = 1;

out vec3 normal;
out vec3 colour;

void main(){
    normal = normalize(inNorm * mat3(inVertToWorld));
    if (inUseSingleColour == 1)
        colour = inColour;
    else
        colour = inVertColour;
    gl_Position = inVert * inVertToWorld * inWorldToCamera * inCameraToClip;
}
