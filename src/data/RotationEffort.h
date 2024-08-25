/** 
 * RotationEffort.h
 * created 08/25/24 by frank collebrusco 
 */
#ifndef ROTATION_EFFORT_H
#define ROTATION_EFFORT_H
#include "util/math.h"
#include "ECS.h"

struct c_RotationEffort {
    alpiv_value<> r;
    float Ki, Kp, Kl;

    static void execute(ECS& ecs, float const dt);
};

#endif /* ROTATION_EFFORT_H */
