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
#include "game/Sprite.h"
#include "ECS.h"

/**
 * one frame of an animation
 * blpos in UV space
 * t = seconds in 2^8 fixed point, so 0s - 1s max
 */
struct anim_frame_t {
    c_StaticAtlas cstat;
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
 * Animation container
 * This is the design interface, where anims are written
 */
struct Animations {
    static constexpr Animation<6> character = {
        .frame_size = Sprites::char_0.size,
        .frames = {
            {Sprites::char_0.at, 1.f/12.f},
            {Sprites::char_1.at, 1.f/12.f},
            {Sprites::char_2.at, 1.f/12.f},
            {Sprites::char_3.at, 1.f/12.f},
            {Sprites::char_4.at, 1.f/12.f},
            {Sprites::char_5.at, 1.f/12.f}
        }
    };
};

/**
 * Animation State Component
 * carries out frame switching through a given animation
 * system must execute before render (redundant)
 * plays in a loop. need done cb?
 */
#include <stdio.h>
struct c_AnimationState {
    float tmr;
    uint8_t idx;
    const uint8_t N;
    const glm::vec2 frame_size;
    const anim_frame_t *const frames;
    template <uint8_t n>
    c_AnimationState(Animation<n> const*const anim) :
        tmr(0.f),
        idx(0),
        N(n),
        frame_size(anim->frame_size),
        frames((const anim_frame_t* const)(anim->frames))
    {
    }

    NO_COPY_OR_MOVE(c_AnimationState);

    bool step(float dt);

    uint8_t get_frame(c_StaticAtlas* dest = 0);

    static void execute(float dt, ECS& ecs);

};


#endif /* ANIMATION_H */
