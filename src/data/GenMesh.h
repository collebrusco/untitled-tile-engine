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
    virtual void step(float const dt, float const t) = 0;
};

struct c_GenMesh final : public c_abstract<GenMesh, 256> {
    static void execute(float const dt, float const t, ECS& ecs);
};

#endif /* GEN_MESH_H */
