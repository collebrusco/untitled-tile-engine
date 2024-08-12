/** 
 * Animation.h
 * created 08/11/24 by frank collebrusco 
 */
#ifndef ANIMATION_H
#define ANIMATION_H
#include <stdint.h>
#include <flgl/glm.h>
#include "util/misc.h"
#include "data/components.h"

/**
 * one frame of an animation
 * blpos in UV space
 * t = seconds in 2^8 fixed point, so 0s - 1s max
 */
struct anim_frame_t {
    glm::vec2 blpos;
    float t;
};
ASSERT_AGGREGATE(anim_frame_t);

/**
 * Animation Type Objects
 * these are used to create and declare animations of N frames
 * this is the design interface
 */
template <uint8_t N>
struct Animation {
    const glm::vec2 frame_size;
    const anim_frame_t frames[N];
};

/**
 * Animation State Component
 * carries out frame switching through a given animation
 * system must execute before render (redundant)
 * plays in a loop. need done cb?
 */
struct c_AnimationState {
    float tmr{0};
    uint8_t idx{0};
    const uint8_t N;
    const glm::vec2 frame_size;
    const anim_frame_t *const frames;
    template <uint8_t n>
    inline c_AnimationState(Animation<n> const& anim) :
        N(n),
        frame_size(anim.frame_size),
        frames(anim.frames)
        {}

    bool inline step(float dt) {
        tmr += dt;
        if (tmr > frames[idx].t) {
            tmr = 0.f; 
            idx = (idx+1)%N;
            return true;
        }
        return false;
    }

    uint8_t inline get_frame(c_StaticAtlas* dest = 0) {
        if (dest) {
            (*(dest)) = c_StaticAtlas::from_sheet(frames[idx].blpos, frames[idx].blpos + frame_size);
        }
        return idx;
    }

    static inline void execute(float dt, ECS& ecs) {
        for (auto e : ecs.view<c_AnimationState>()) {
            auto& as = ecs.getComp<c_AnimationState>(e);
            if (as.step(dt))
                as.get_frame(&(ecs.getComp<c_StaticAtlas>(e)));
        }
    }

};

struct Animations {

};


#endif /* ANIMATION_H */
