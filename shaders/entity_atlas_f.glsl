#version 410 core

in vec2 iUV;

uniform sampler2D utex;
uniform vec2 uBL;

out vec4 out_color;

#define SHEET_PIX_F (1024.f)
void main() {
    vec2 uv = uBL + (iUV / SHEET_PIX_F);
    uv.y = 1.f - uv.y;
    vec4 c = texture(utex, uv);
    if (c.w < 1.) discard;
    out_color = c;
}
