/** 
 * GenMesh.h
 * created 08/15/24 by frank collebrusco 
 */
#ifndef GEN_MESH_H
#define GEN_MESH_H
#include <flgl.h>
#include "util/c_abstract.h"
#include "ECS.h"

struct GenMesh {
    virtual void sync(VertexBuffer<Vt_classic>& vbo, ElementBuffer& ibo) const = 0;
    virtual void step(float const dt) = 0;
};

struct c_GenMesh final : public c_abstract<GenMesh, 128> {
    static void execute(float const dt, ECS& ecs);
};

struct TMesh final : public GenMesh {
    float t{0};
    virtual void sync(VertexBuffer<Vt_classic>& vbo, ElementBuffer& ibo) const override final;
    virtual void step(float const dt) override final;
};

#endif /* GEN_MESH_H */
