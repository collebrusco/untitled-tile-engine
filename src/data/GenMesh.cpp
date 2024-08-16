#include "GenMesh.h"
#include <flgl/logger.h>
LOG_MODULE(gm);

void c_GenMesh::execute(float const dt, ECS &ecs) {
    for (auto e : ecs.view<c_GenMesh>()) {
        ecs.getComp<c_GenMesh>(e).get().step(dt);
    }
}

void TMesh::sync(VertexBuffer<Vt_classic>& vbo, ElementBuffer& ibo) const {
    Vt_classic verts[8];
    uint32_t elems[12] = {0, 2, 1, 0, 2, 3, 4+0, 4+2, 4+1, 4+0, 4+2, 4+3};
    verts[0] = {{-0.5, 0, 0}, {0.5,0.5}};
    verts[1] = {{-0.5, 0.5 * cos(10.f * t), 0}, {0.5,0.5}};
    verts[2] = {{0, 0.5 * cos(10.f * t), 0}, {0.5,0.5}};
    verts[3] = {{0, 0, 0}, {0.5,0.5}};
    verts[4] = {{0, 0, 0}, {0.5,0.5}};
    verts[5] = {{0, -0.5 * cos(10.f * t), 0}, {0.5,0.5}};
    verts[6] = {{0.5, -0.5 * cos(10.f * t), 0}, {0.5,0.5}};
    verts[7] = {{0.5, 0, 0}, {0.5,0.5}};

    // verts[0] = {{-0.5, -0.5, 0}, {0.5,0.5}};
    // verts[1] = {{-0.5,  0.5, 0}, {0.5,0.5}};
    // verts[2] = {{ 0.5,  0.5, 0}, {0.5,0.5}};
    // verts[3] = {{ 0.5, -0.5, 0}, {0.5,0.5}};

    vbo.bind();
    vbo.buffer_data(8, verts);
    vbo.unbind();

    ibo.bind();
    ibo.buffer_data(12, elems);
    ibo.unbind();
}

void TMesh::step(float const dt) {
    t += dt;
}

void HumanoidMesh::sync(VertexBuffer<Vt_classic> &vbo, ElementBuffer &ibo) const {
    Vt_classic verts[8];
    uint32_t elems[12] = {0, 2, 1, 0, 2, 3, 4+0, 4+2, 4+1, 4+0, 4+2, 4+3};
    static float lp = 0;

    lp = lp + (target_leg_pos - lp) * 0.8;
    verts[0] = {{-0.4, 0  , -0.1f}, {0.5,0.5}};
    verts[1] = {{-0.4, lp , -0.1f}, {0.5,0.5}};
    verts[2] = {{0   , lp , -0.1f}, {0.5,0.5}};
    verts[3] = {{0   , 0  , -0.1f}, {0.5,0.5}};
    verts[4] = {{0   , 0  , -0.1f}, {0.5,0.5}};
    verts[5] = {{0   , -lp, -0.1f}, {0.5,0.5}};
    verts[6] = {{0.4 , -lp, -0.1f}, {0.5,0.5}};
    verts[7] = {{0.4 , 0  , -0.1f}, {0.5,0.5}};

    vbo.bind();
    vbo.buffer_data(8, verts);
    vbo.unbind();

    ibo.bind();
    ibo.buffer_data(12, elems);
    ibo.unbind();
}

void HumanoidMesh::step(float const dt) {
    static float t = 0;
    static bool pwalkin = false;
    if (walkin) {
        if (!pwalkin) t = 0;
        t += dt;
        target_leg_pos = ((runnin) ? 0.7 : 0.4) * sin((runnin ? 16.f : 12.f) * t);
    } else {
        target_leg_pos -= target_leg_pos * 0.7;
    }

    pwalkin = walkin;
}
