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
};

#endif /* SPRITE_H */
