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

#define ACTION_MAX_ACTIONS  1024
#define ACTION_MAX_SIZE     (64-1)
#define ACTOR_MAX_SIZE      64

struct Action;
typedef abstract_freelist_objpool<Action, ACTION_MAX_SIZE, ACTION_MAX_ACTIONS> ActionPool;


struct Action {
    virtual ~Action() = default;
    virtual void perform() = 0;
    virtual bool complete() const = 0;
};

struct Actor {
    virtual ~Actor() = default;
    virtual void take_turn(entID self, World *const world, ActionPool* const apool) = 0;
};


struct c_Actor final : public c_abstract<Actor, ACTOR_MAX_SIZE> {

    static void take_all_turns(World*const ecs, ActionPool* const apool);

};


#endif /* ACTOR_H */
