/** 
 * HumanoidMesh.h
 * created 08/16/24 by frank collebrusco 
 */
#ifndef HUMANOID_MESH_H
#define HUMANOID_MESH_H
#include <flgl.h>
#include "ECS.h"
#include "data/GenMesh.h"

enum hoid_leg_state_e {
    STOOD = 0,
    WALKING,
    RUNNING
};
struct HumanoidLegs {
    static const uint8_t num_verts = 8;
    static const uint8_t num_elems = 12;
    float target_pos{0};
    float pos{0};
    hoid_leg_state_e state  : 4;
    // hoid_leg_state_e pstate : 4;
    void sync(Vt_classic *const verts, uint16_t *const vtop, 
              uint32_t *const elems, uint16_t *const etop) const;
    void step(float const dt, float const t);
};

struct HumanoidMesh final : public GenMesh {
    virtual void sync(VertexBuffer<Vt_classic>& vbo, ElementBuffer& ibo) const override final;
    virtual void step(float const dt, float const t) override final;

    HumanoidLegs legs;

};

#endif /* HUMANOID_MESH_H */
