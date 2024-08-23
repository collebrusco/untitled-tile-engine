/** 
 * HumanoidMesh.h
 * created 08/16/24 by frank collebrusco 
 */
#ifndef HUMANOID_MESH_H
#define HUMANOID_MESH_H
#include <flgl.h>
#include "ECS.h"
#include "data/GenMesh.h"

struct HumanoidMesh final : public GenMesh {
    virtual void sync(VertexBuffer<Vt_classic>& vbo, ElementBuffer& ibo) const override final;
    virtual void step(float const dt, float const t) override final;

    struct Head {
        static const uint8_t num_verts = 4;
        static const uint8_t num_elems = 6;
        void sync(Vt_classic *const verts, uint16_t *const vtop, 
                uint32_t *const elems, uint16_t *const etop) const;
    } head;
    
    struct Legs {
        static const uint8_t num_verts = 8;
        static const uint8_t num_elems = 12;
        float target_pos{0};
        float pos{0};
        enum state_e {
            STOOD = 0,
            WALKING,
            RUNNING
        } state  : 4;
        void sync(Vt_classic *const verts, uint16_t *const vtop, 
                uint32_t *const elems, uint16_t *const etop) const;
        void step(float const dt, float const t);
    } legs;

    struct Torso {
        static const uint8_t num_verts = 4;
        static const uint8_t num_elems = 6;
        void sync(Vt_classic *const verts, uint16_t *const vtop,
                uint32_t *const elems, uint16_t *const etop) const;
    } torso;

    struct Arms {
        static const uint8_t num_verts = 16;
        static const uint8_t num_elems = 24;
        float target_pos{0};
        float pos{0};
        enum state_e {
            STOOD = 0,
            WALKING,
            RUNNING,
            AIMING,
        } state  : 4;
        void sync(Vt_classic *const verts, uint16_t *const vtop, 
                uint32_t *const elems, uint16_t *const etop) const;
        void step(float const dt, float const t);
    } arms;

};

#endif /* HUMANOID_MESH_H */
