#version 410 core

out vec4 outColor;

uniform sampler2D utex;

in vec2 iUV;
in vec3 iPos;

void main(){
    outColor = vec4(1.);//texture(utex, iUV);
    outColor = texture(utex, iUV);
}
