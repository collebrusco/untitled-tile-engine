#include "Animation.h"
#include <flgl/logger.h>
LOG_MODULE(anim);

bool c_AnimationState::step(float dt) {
    tmr += dt;
    if (tmr > frames[idx].t) {
        tmr = 0.f; 
        idx = (idx+1)%N;
        return true;
    }
    return false;
}

uint8_t c_AnimationState::get_frame(c_StaticAtlas* dest) {
    if (dest) {
        (*(dest)) = frames[idx].cstat;
    }
    return idx;
}

void c_AnimationState::execute(float dt, ECS& ecs) {
    for (auto e : ecs.view<c_AnimationState>()) {
        auto& as = ecs.getComp<c_AnimationState>(e);
        if (as.step(dt))
            as.get_frame(&(ecs.getComp<c_StaticAtlas>(e)));
    }
}
