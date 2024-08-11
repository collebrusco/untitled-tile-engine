/** 
 * Action.h
 * created 08/08/24 by frank collebrusco 
 */
#ifndef ACTION_H
#define ACTION_H
#include "util/abstract_freelist_objpool.h"

#define ACTION_MAX_ACTIONS  1024
#define ACTION_MAX_SIZE     (64-1)

struct Action;
typedef abstract_freelist_objpool<Action, ACTION_MAX_SIZE, ACTION_MAX_ACTIONS> ActionPool;

struct Action {
    virtual ~Action() = default;
    virtual void perform() = 0;
    virtual bool complete() const = 0;
};

#endif /* ACTION_H */
