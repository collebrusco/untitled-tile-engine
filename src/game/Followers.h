/** 
 * Followers.h
 * created 08/08/24 by frank collebrusco 
 */
#ifndef FOLLOWERS_H
#define FOLLOWERS_H
#include "util/misc.h"
#include "ECS.h"

struct c_DiffFollower {
    entID tar;
    float pct;

    static void execute(ECS& ecs);
};
ASSERT_AGGREGATE(c_DiffFollower);


#endif /* FOLLOWERS_H */
