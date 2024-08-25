/** 
 * oil.h
 * created 08/25/24 by frank collebrusco 
 */
#ifndef OIL_H
#define OIL_H
#include <flgl/glm.h>
#include "data/Region.h"

struct c_oilrig {
    float reserve;

    static void place(ECS& ecs, SurfaceTile& st);
    static void execute(ECS& ecs, float const dt);
};

#endif /* OIL_H */
