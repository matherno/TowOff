#version 330

#define DRAW_STYLE_SINGLE_COLOUR  1
#define DRAW_STYLE_TEXTURE        2
#define DRAW_STYLE_VERT_COLOUR    3

layout(location = 0) in vec4 inVert;
layout(location = 1) in vec3 inNorm;
layout(location = 2) in vec3 inVertColour;
layout(location = 3) in vec2 inTexCoord;

uniform mat4 inVertToWorld = mat4(1);
uniform mat4 inWorldToCamera = mat4(1);
uniform mat4 inCameraToClip = mat4(1);
uniform vec3 inColour;
uniform int inDrawStyle = DRAW_STYLE_SINGLE_COLOUR;
uniform int inClippingEnabled = 0;
uniform vec4 inClipPlane;

centroid out vec3 normal;
centroid out vec3 colour;
centroid out vec2 texCoords;

void performClipping(vec4 worldSpaceVert){
  gl_ClipDistance[0] = dot(worldSpaceVert, inClipPlane);
}

void main(){
    normal = normalize(inNorm * mat3(inVertToWorld));
    colour = inColour;

    if (inDrawStyle == DRAW_STYLE_TEXTURE)
        texCoords = inTexCoord;
    else if (inDrawStyle == DRAW_STYLE_VERT_COLOUR)
        colour = inVertColour;

    vec4 worldVertex = inVert * inVertToWorld;
    if (inClippingEnabled > 0)
      performClipping(worldVertex);
    gl_Position = worldVertex * inWorldToCamera * inCameraToClip;
}
