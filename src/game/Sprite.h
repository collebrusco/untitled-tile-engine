/** 
 * Sprite.h
 * created 08/11/24 by frank collebrusco 
 */
#ifndef SPRITE_H
#define SPRITE_H
#include "data/components.h"

struct Sprite {
    const c_StaticAtlas at;
    const glm::vec2 size;
};


#define SPRITE_DECL(blx, bly, sx, sy) ((Sprite) {.at = c_StaticAtlas::from_sheet({(blx), (bly)}, {(sx), (sy)}), .size = {sx, sy} })

struct Sprites {
    static constexpr Sprite temp_player =   SPRITE_DECL(63.f + 3.f/16.f, 3.f/16.f, 10.f/16.f, 10.f/16.f);
    static constexpr Sprite char_0 =        SPRITE_DECL((0.f * (24.f / 16.f)), (0.f), (1.5f), (2.f));
    static constexpr Sprite char_1 =        SPRITE_DECL((1.f * (24.f / 16.f)), (0.f), (1.5f), (2.f));
    static constexpr Sprite char_2 =        SPRITE_DECL((2.f * (24.f / 16.f)), (0.f), (1.5f), (2.f));
    static constexpr Sprite char_3 =        SPRITE_DECL((3.f * (24.f / 16.f)), (0.f), (1.5f), (2.f));
    static constexpr Sprite char_4 =        SPRITE_DECL((4.f * (24.f / 16.f)), (0.f), (1.5f), (2.f));
    static constexpr Sprite char_5 =        SPRITE_DECL((5.f * (24.f / 16.f)), (0.f), (1.5f), (2.f));
    static constexpr Sprite char_6 =        SPRITE_DECL((6.f * (24.f / 16.f)), (0.f), (1.5f), (2.f));
};

#endif /* SPRITE_H */
