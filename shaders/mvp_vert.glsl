#version 410 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec2 iUV;

void main() {
    iUV = aUV;
    
    gl_Position = uProj * uView * uModel * vec4(aPos, 1.0f);
}
