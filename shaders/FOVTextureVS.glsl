#version 330

layout(location = 0) in vec2 inVert;

uniform mat4 inVertToWorld = mat4(1);
uniform mat4 inWorldToCamera = mat4(1);
uniform mat4 inCameraToClip = mat4(1);

out vec2 worldPos;    // x,z

void main(){
    vec4 worldVertex = vec4(inVert.x, 0, inVert.y, 1) * inVertToWorld;
    worldPos = worldVertex.xz;
    gl_Position = worldVertex * inWorldToCamera * inCameraToClip;
    gl_Position.z = -1;
}
