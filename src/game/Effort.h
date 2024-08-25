/** 
 * Effort.h
 * created 08/25/24 by frank collebrusco 
 */
#ifndef EFFORT_H
#define EFFORT_H
#include "util/math.h"
#include "ECS.h"

struct c_RotationEffort {
    alpiv_value<> r;
    float Ki, Kp, Kl;

    static void execute(ECS& ecs, float const dt);
};

#endif /* EFFORT_H */
