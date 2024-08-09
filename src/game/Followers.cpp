#include "Followers.h"
#include "components.h"

void c_DiffFollower::execute(ECS& ecs) {
    for (auto e : ecs.view<c_DiffFollower, c_Object>()) {
        auto& flw = ecs.getComp<c_DiffFollower>(e);
        auto& obj = ecs.getComp<c_Object>(e);
        auto opos = ecs.getComp<c_Object>(flw.tar).pos;
        obj.pos += ((opos - obj.pos) * flw.pct);
    }
}

