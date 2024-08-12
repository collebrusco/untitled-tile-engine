#include "Animation.h"
#include "game/Sprite.h"
#include <flgl/logger.h>
LOG_MODULE(anim);

const Animation<6> Animations::character({
    .frame_size = Sprites::char_0.size,
    .frames = {
        {Sprites::char_0.at.copy(), 1.f/12.f},
        {Sprites::char_1.at.copy(), 1.f/12.f},
        {Sprites::char_2.at.copy(), 1.f/12.f},
        {Sprites::char_3.at.copy(), 1.f/12.f},
        {Sprites::char_4.at.copy(), 1.f/12.f},
        {Sprites::char_5.at.copy(), 1.f/12.f}
    }
});

bool c_AnimationState::step(float dt) {
    tmr += dt;
    if (tmr > frames[idx].t) {
        tmr = 0.f; 
        idx = (idx+1)%N;
        LOG_INF("stepping i = %d frame %.2f, %.2f t=%.2f   to      %.2f, %.2f   t=%.2f", idx, frames[idx].cstat.uvs.bl.x, frames[idx].cstat.uvs.bl.y, frames[idx].t, frames[idx].cstat.uvs.tr.x, frames[idx].cstat.uvs.tr.y, frames[idx].t);
        LOG_INF("%p: anims::char.frames: %p, this frame is %p", this, Animations::character.frames, frames);
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
