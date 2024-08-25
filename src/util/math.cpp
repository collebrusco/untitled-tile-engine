#include "math.h"

glm::vec2 rotate_around_origin(glm::vec2 pt, float deg) {
    return {
        (pt.x * glm::cos(glm::radians(deg))) - (pt.y * (glm::sin(glm::radians(deg)))),
        (pt.x * glm::sin(glm::radians(deg))) + (pt.y * (glm::cos(glm::radians(deg)))),
    };
}

glm::vec2 rotate_around_pt(glm::vec2 pt, glm::vec2 c, float deg) {
    return c + rotate_around_origin(pt - c, deg);
}

glm::vec3 rotate_around_origin(glm::vec3 pt, float deg) {
    return glm::vec3(
        (pt.x * glm::cos(glm::radians(deg))) - (pt.y * (glm::sin(glm::radians(deg)))),
        (pt.x * glm::sin(glm::radians(deg))) + (pt.y * (glm::cos(glm::radians(deg)))),
        pt.z
    );
}

glm::vec3 rotate_around_pt(glm::vec3 pt, glm::vec2 c, float deg) {
    return glm::vec3(c, pt.y) + rotate_around_origin(pt - glm::vec3(c, pt.y), deg);
}
float circle_diff(float Vt, float Vi, float max) {
    Vt = posimod(Vt); Vi = posimod(Vi);
    float v = Vt - Vi;
    float vh = (Vt - max) - Vi;
    float vl = (Vt + max) - Vi;
    float r = amin(amin(v, vh), vl);
    return r;
}

float posimod(float x, float min, float max) {
    while (x > max) {x -= (max-min);}
    while (x < min) {x += (max-min);}
    return x;
}
