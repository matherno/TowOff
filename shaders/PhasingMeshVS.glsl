#version 330

layout(location = 0) in vec4 inVert;

uniform mat4 inVertToWorld = mat4(1);
uniform mat4 inWorldToCamera = mat4(1);
uniform mat4 inCameraToClip = mat4(1);
uniform int inClippingEnabled = 0;
uniform vec4 inClipPlane;

out float height;

void performClipping(vec4 worldSpaceVert){
  gl_ClipDistance[0] = dot(worldSpaceVert, inClipPlane);
}

void main(){
    vec4 worldVertex = inVert * inVertToWorld;
    if (inClippingEnabled > 0)
      performClipping(worldVertex);
    height = worldVertex.y;
    gl_Position = worldVertex * inWorldToCamera * inCameraToClip;
}
