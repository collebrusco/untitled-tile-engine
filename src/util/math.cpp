#include "math.h"

glm::vec2 rotate_around_origin(glm::vec2 pt, float deg)
{
    return {
        (pt.x * glm::cos(glm::radians(deg))) - (pt.y * (glm::sin(glm::radians(deg)))),
        (pt.x * glm::sin(glm::radians(deg))) + (pt.y * (glm::cos(glm::radians(deg)))),
    };
}

glm::vec2 rotate_around_pt(glm::vec2 pt, glm::vec2 c, float deg)
{
    return c + rotate_around_origin(pt - c, deg);
}
