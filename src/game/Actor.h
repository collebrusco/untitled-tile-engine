/** 
 * Actor.h
 * created 07/22/24 by frank collebrusco 
 */
#ifndef ACTOR_H
#define ACTOR_H
#include "ECS.h"
#include "util/abstract_freelist_objpool.h"

#define ACTION_MAX_ACTIONS  1024
#define ACTION_MAX_SIZE     64-1
#define ACTOR_MAX_SIZE      64

struct Action;
typedef abstract_freelist_objpool<Action, ACTION_MAX_SIZE, ACTION_MAX_ACTIONS> ActionPool;

/**
 * virtual action interface
 * perform and ask if complete
 */
struct Action {
    inline virtual ~Action() {}
    virtual void perform() = 0;
    virtual bool complete() const = 0;
};

struct Actor {
    virtual ~Actor() = default;
    virtual void take_turn(ActionPool *apool) = 0;
};

/** 
 * Actor component is storage for Actor impls up to ACTOR_MAX_SIZE bytes
 */
struct c_Actor {

    Actor& get();

    template <typename ActorType, typename ...Args>
    ActorType& emplace(Args... args) {
static_assert(sizeof(ActorType) <= ACTOR_MAX_SIZE);
        new (data) ActorType(args...);
        return this->get();
    }

    void destroy();

    template <typename ActorType, typename ...Args>
    ActorType& replace(Args... args) {
        this->destroy();
        return this->emplace(args...);
    }

    static void take_all_turns(ECS* ecs, ActionPool* apool);

private:
    alignas(Actor) char data[ACTOR_MAX_SIZE];
};


#endif /* ACTOR_H */
