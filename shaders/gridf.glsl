#version 410 core

out vec4 outColor;

in vec2 iUV;
in vec3 iPos;

uniform float uVW;
uniform float uAspect;
uniform vec2 uMP;

#define THRESH (0.02)

void main(){
    float x = fract(iUV.x * uVW);
    float y = fract(iUV.y * (uVW / uAspect));
    vec4 c = vec4(1.);
    if (x < THRESH || x > (1.-THRESH) || y < THRESH || y > (1.-THRESH)) {
        c = mix(vec4(vec3(0.3), 1.), c, distance(uMP, vec2(iUV.x, iUV.y / uAspect)) * 4.);
    }
    outColor = c;
}
