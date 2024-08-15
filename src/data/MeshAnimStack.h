/** 
 * MeshAnimStack.h
 * created 08/14/24 by frank collebrusco 
 */
#ifndef MESH_ANIM_STACK_H
#define MESH_ANIM_STACK_H
#include "ECS.h"
#include "util/abstract_linear_objpool.h"
#include "game/Sprite.h"
#include <flgl.h>
#include "data/Animation.h"

struct AbsAnimStackLayer {
    virtual bool step(float dt) = 0;
    virtual void add_mesh(Vt_classic* verts, uint16_t* vtop, uint32_t* elems, uint16_t* etop, float z) const = 0;
    /** TODO: what's the general way to give a layer state? sub to event? dont generalize it? */

protected:
    /** needs verts bl to up, right, down */
    static void add_quad_idxs(uint32_t* elems, uint16_t* etop);
};

class c_AnimationStack final : public abstract_linear_objpool<AbsAnimStackLayer, 256> {
public:
    /** step the anims and if any changed mesh is rebuilt */
    bool exec(float dt, Vt_classic* verts, uint16_t* vtop, uint32_t* elems, uint16_t* etop);
};


/**
 * Animation State Component
 * carries out frame switching through a given animation
 * system must execute before render (redundant)
 * plays in a loop. need done cb?
 */
struct HandAnimStackLayer final : public AbsAnimStackLayer {
    float tmr;
    uint8_t idx;
    const uint8_t N;
    const glm::vec2 frame_size;
    /** this is a cache miss anyway probably so whats the point? better to copy? would need c_abstract */
    const anim_frame_t *const frames;
    NO_COPY_OR_MOVE(HandAnimStackLayer);
    template <uint8_t n>
    HandAnimStackLayer(Animation<n> const*const anim) :
        tmr(0.f),
        idx(0),
        N(n),
        frame_size(anim->frame_size),
        frames((const anim_frame_t* const)(anim->frames)) {}

    virtual bool step(float dt) override final;
    virtual void add_mesh(Vt_classic* verts, uint16_t* vtop, uint32_t* elems, uint16_t* etop, float z) const override final;

};


#endif /* MESH_ANIM_STACK_H */
