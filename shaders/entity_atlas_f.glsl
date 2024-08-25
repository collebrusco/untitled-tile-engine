#version 410 core

in vec2 iUV;

uniform sampler2D utex;

out vec4 out_color;

void main() {
    vec4 c = texture(utex, iUV);
    if (c.w < 1.) discard;
    out_color = c;
    out_color = vec4(1.f);
}
