#version 410 core

out vec4 outColor;
in vec2 iUV;
in vec3 iPos;

uniform float uTime;
uniform vec2 uRes;
uniform float uAspect;
uniform vec2 uCampos;

vec2 n_res = gl_FragCoord.xy / uRes;
vec2 i_res = vec2(n_res.x * uAspect, n_res.y);

float rand(vec2 co){
    return sin(dot(co, vec2(12.9898, 78.233)) * 43758.5453);
}

// smootherstep, ken perlin
float interp(float x){
    return ((x * (x * 6.0 - 15.0) + 10.0) * x * x * x);
}

// smootherstep between a and b
float interp(float a, float b, float x){
    float ab = (b - a);
    return (ab * interp(x)) + a;
}

// produces a "random" unit vector by a 2-dimensional seed
// random function based on a wikipedia implementation
// shuffles the bits of integer inputs to create theta
vec2 gradient(int x, int y){
    uint a = x;
    uint b = y;
    a *= 0xBAF3C90F;
    b ^= (a << 16) | (a >> 16);
    b *= 0x5454FAFF;
    a ^= (b << 16) | (b >> 16);
    a *= 0xDA442859;
    float theta = a * (3.14159265 / (2 * (0x10000000)));
    return vec2(cos(theta), sin(theta));
}

// identical to above gradient function, but rotates all vectors by time offset
vec2 gradient(int x, int y, float speed){
    uint a = x;
    uint b = y;
    a *= 0xBAF3C90F;
    b ^= (a << 16) | (a >> 16);
    b *= 0x5454FAFF;
    a ^= (b << 16) | (b >> 16);
    a *= 0xDA442859;
    float theta = a * (3.14159265 / (2 * (0x10000000)));
    return vec2(cos(theta+(uTime*speed)), sin(theta+(uTime*speed)));
}

float perlin(vec2 pos, float rotation, vec2 offset, vec2 scale){
    // translate, scale
    pos += offset;
    pos *= scale;
    
    // establish grid
    int x0 = int(floor(pos.x));
    int y0 = int(floor(pos.y));
    int x1 = x0 + 1;
    int y1 = y0 + 1;
    
    // offset vectors, from grid corners to fragment
    vec2 botL = (pos - vec2(float(x0), float(y0))) / 1.41;
    vec2 topR = (pos - vec2(float(x1), float(y1))) / 1.41;
    vec2 topL = (pos - vec2(float(x0), float(y1))) / 1.41;
    vec2 botR = (pos - vec2(float(x1), float(y0))) / 1.41;
    
    // dot each offset vector with random gradient at each grid corner
    float BL = dot(botL, gradient(x0, y0, rotation));
    float TR = dot(topR, gradient(x1, y1, rotation));
    float TL = dot(topL, gradient(x0, y1, rotation));
    float BR = dot(botR, gradient(x1, y0, rotation));
    
    // interpolate, scale 0 to 1
    return (interp(interp(BL, BR, pos.x - floor(pos.x)), interp(TL, TR, pos.x - floor(pos.x)), pos.y - floor(pos.y)) * 0.5) + 0.5;
}

float perlin(vec2 pos, float rotation, vec2 offset){
    return perlin(pos, rotation, offset, vec2(1));
}

float perlin(vec2 pos, float rotation){
    return perlin(pos, rotation, vec2(0), vec2(1));
}

float perlin(vec2 pos){
    return perlin(pos, 0, vec2(0), vec2(1));
}

#define NP 5

#define CP_DIV 100.

float agg(float scale, int n) {
    float aggregate = 0.; 
    float divisor = 0.;
    for (int i = 2; i <= NP+1; i++) {
        float mult = 8. / (i * i);
        float p = perlin(
                            i_res, 
                            0.2,
                            (i/4.) * ((uCampos / CP_DIV) + vec2((n + i) * 644)),
                            scale * vec2((i*i))
                        );
        aggregate += p * mult;
        divisor += mult;
    }
    aggregate /= divisor;
    return aggregate;
}

vec4 bgcolor() {
    int n = 1;

    float red   = agg(1., n++) / 12.;
    float green = 0; //agg(3., n++) / 18.;
    float blue  = agg(1., n++) / 12.;

    float base  = agg(0.5, n++) / 8.;

    return vec4(base + red, base + green, base + blue, 1.f);
}

void main(){
    outColor = bgcolor();
}
