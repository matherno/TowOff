#version 330

layout(location = 0) in vec4 inVert;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec2 inTexCoord;

uniform mat4 inVertToWorld = mat4(1);
uniform mat4 inWorldToCamera = mat4(1);
uniform mat4 inCameraToClip = mat4(1);

out vec3 normal;

void main(){
    normal = normalize(inNorm * mat3(inVertToWorld));
    gl_Position = inVert * inVertToWorld * inWorldToCamera * inCameraToClip;
}
