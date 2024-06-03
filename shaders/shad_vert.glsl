#version 410 core
layout (location = 0) in vec3 apos;
layout (location = 1) in vec3 anorm;

uniform vec2 ulpos;
uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;

void main() {
    vec4 vpos = uModel * vec4(apos, 1.f);
    vec2 dlight = ulpos - vpos.xy;
    vec2 norm = anorm.xy;
    float dt = dot(dlight, norm);
    if (dt<0.) {
        vpos.xy -= 10000.f*normalize(dlight);
    }

    vpos = uProj * uView * vpos;

    gl_Position = vpos;


    // gl_Position = uProj * (uView * (uModel * vec4(apos, 1.f)));
}
