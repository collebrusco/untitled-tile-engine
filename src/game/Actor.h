/** 
 * Actor.h
 * created 07/22/24 by frank collebrusco 
 */
#ifndef ACTOR_H
#define ACTOR_H
#include "ECS.h"
#include "util/abstract_freelist_objpool.h"
#include "util/c_abstract.h"
#include "World.h"
#include "State.h"
#include "game/world_mouse.h"

#define ACTOR_MAX_SIZE      64

struct Actor {
    virtual ~Actor() = default;
    virtual void take_turn(entID self, State& state, Keyboard const& kb, world_mouse_t const& wm) = 0;
};


struct c_Actor final : public c_abstract<Actor, ACTOR_MAX_SIZE> {

    static void take_all_turns(State& state, Keyboard const& kb, world_mouse_t const& wm);

};


#endif /* ACTOR_H */
