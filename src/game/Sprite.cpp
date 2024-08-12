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


const constexpr Sprite Sprites::char_0 = declare((0.f * (24.f / 16.f)), (0.f), (1.5f), (2.f));
const constexpr Sprite Sprites::char_1 = declare((1.f * (24.f / 16.f)), (0.f), (1.5f), (2.f));
const constexpr Sprite Sprites::char_2 = declare((2.f * (24.f / 16.f)), (0.f), (1.5f), (2.f));
const constexpr Sprite Sprites::char_3 = declare((3.f * (24.f / 16.f)), (0.f), (1.5f), (2.f));
const constexpr Sprite Sprites::char_4 = declare((4.f * (24.f / 16.f)), (0.f), (1.5f), (2.f));
const constexpr Sprite Sprites::char_5 = declare((5.f * (24.f / 16.f)), (0.f), (1.5f), (2.f));
const constexpr Sprite Sprites::char_6 = declare((6.f * (24.f / 16.f)), (0.f), (1.5f), (2.f));