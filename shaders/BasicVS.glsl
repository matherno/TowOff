#version 330

layout(location = 0) in vec4 inPosition;

out vec2 colour;

void main(){
    colour = vec2(inPosition.x * 0.5 + 0.5, inPosition.y + 0.5);
    gl_Position = inPosition;
}
