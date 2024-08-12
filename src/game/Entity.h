/** 
 * Entity.h
 * created 08/11/24 by frank collebrusco
 */
#ifndef ENTITY_H
#define ENTITY_H
#include "data/State.h"
#include "game/Sprite.h"

struct Entity {
    static void config_for_sprite(entID e, ECS& ecs, Sprite sprite);
};

#endif /* ENTITY_H */
