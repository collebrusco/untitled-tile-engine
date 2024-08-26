/** 
 * Entity.h
 * created 08/11/24 by frank collebrusco
 * for now i place common entity helper funcs here
 */
#ifndef ENTITY_H
#define ENTITY_H
#include "data/State.h"
#include "game/Sprite.h"
#include "data/Animation.h"

struct Entity {
    static void config_for_sprite(entID e, ECS& ecs, Sprite sprite, float scale=1.f);
    template <uint8_t n>
    static void set_anim_if_not(entID e, ECS& ecs, Animation<n> const*const anim);
};


template <uint8_t n>
void Entity::set_anim_if_not(entID e, ECS& ecs, Animation<n> const*const anim) {
    c_AnimationState* astate = ecs.tryGetComp<c_AnimationState>(e);
    if (!astate || (astate->frames != anim->frames)) {
        c_AnimationState& astate = ecs.addComp<c_AnimationState>(e, anim);
        c_EntRenderEntry* ere = ecs.tryGetComp<c_EntRenderEntry>(e);
        if (!ere) ecs.addComp<c_EntRenderEntry>(e);
        ere = &ecs.getComp<c_EntRenderEntry>(e);
        astate.get_frame(ere);
    }
}

#endif /* ENTITY_H */
