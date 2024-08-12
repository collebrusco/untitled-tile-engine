#include "Sprite.h"
using namespace glm;

static constexpr Sprite declare(float blx, float bly, float sx, float sy) {
    return (Sprite) {
        .at = c_StaticAtlas::from_sheet({(blx), (bly)}, {(sx), (sy)}),
        .size = {sx, sy}
    };
}

static constexpr Sprite declare(float blx, float bly, float s) {
    return declare(blx, bly, s, s);
}

const constexpr Sprite Sprites::temp_player = declare(63.f + 3.f/16.f, 3.f/16.f, 10.f/16.f);

