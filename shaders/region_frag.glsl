#version 410 core

in vec2 iUV;

out vec4 outColor;

uniform sampler2D utex;

void main() {
    vec4 clr = texture(utex, iUV);
    outColor = clr;
    // outColor = vec4(iUV, 0.f, 1.f);
}
