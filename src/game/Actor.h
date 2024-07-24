/** 
 * Actor.h
 * created 07/22/24 by frank collebrusco 
 */
#ifndef ACTOR_H
#define ACTOR_H
#include "ECS.h"
#include "util/abstract_freelist_objpool.h"

struct Action {
    inline virtual ~Action() {}
    virtual void perform() = 0;
};

static abstract_freelist_objpool<Action, 63, 256> alist;

typedef void(*actor_f)(entID self);

struct c_Actor {

};


#endif /* ACTOR_H */
