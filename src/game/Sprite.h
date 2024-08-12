/** 
 * Sprite.h
 * created 08/11/24 by frank collebrusco 
 */
#ifndef SPRITE_H
#define SPRITE_H
#include "data/components.h"

struct Sprite {
    c_StaticAtlas at;
    glm::vec2 size;
};

struct Sprites {
    const static Sprite temp_player;

    const static Sprite char_0;
    const static Sprite char_1;
    const static Sprite char_2;
    const static Sprite char_3;
    const static Sprite char_4;
    const static Sprite char_5;
    const static Sprite char_6;
};

#endif /* SPRITE_H */
