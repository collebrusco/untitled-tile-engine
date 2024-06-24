#version 410 core
layout (location = 0) in vec2 aPos;
layout (location = 1) in vec2 aUV;

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

out vec2 iPos;
out vec2 iUV;

void main() {
    iPos = aPos;
    iUV = aUV;
    gl_Position = uProj * uView * uModel * vec4(aPos, 0.f, 1.0f);
}
