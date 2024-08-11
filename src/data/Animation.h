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

template <uint8_t N>
struct Animation {
    glm::vec2 frame_size;
    anim_frame_t frames[N];
};

struct AnimationState {
    float tmr{0};
    uint8_t idx{0};
    const glm::vec2 frame_size;
    const anim_frame_t *const frames;
    const uint8_t N;
    template <uint8_t n>
    inline AnimationState(Animation<n> const& anim) :
        frame_size(anim.frame_size),
        frames(anim.frames),
        N(n)
        {}

    void inline step(float dt) {
        tmr += dt;
        if (tmr > frames[idx].t) {
            tmr = 0.f; 
            idx = (idx+1)%N;
        }
    }

    uint8_t inline get_frame(c_StaticAtlas* dest = 0) {
        if (dest) {
            dest->uvs.bl = frames[idx].blpos;
            dest->uvs.tr = frames[idx].blpos + frame_size;
        }
        return idx;
    }
};


#endif /* ANIMATION_H */
