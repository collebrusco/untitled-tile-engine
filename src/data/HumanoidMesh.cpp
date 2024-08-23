#include "HumanoidMesh.h"
#include "util/math.h"

void HumanoidMesh::sync(VertexBuffer<Vt_classic> &vbo, ElementBuffer &ibo) const {
    const uint16_t nv = HumanoidMesh::Legs::num_verts + HumanoidMesh::Torso::num_verts + HumanoidMesh::Arms::num_verts + HumanoidMesh::Head::num_verts;
    const uint16_t ne = HumanoidMesh::Legs::num_elems + HumanoidMesh::Torso::num_elems + HumanoidMesh::Arms::num_elems + HumanoidMesh::Head::num_elems;
    Vt_classic verts[nv];
    uint32_t elems[ne];
    uint16_t vt = 0, et = 0;
    legs.sync(verts, &vt, elems, &et);
    torso.sync(verts, &vt, elems, &et);
    arms.sync(verts, &vt, elems, &et);
    head.sync(verts, &vt, elems, &et);


    vbo.bind();
    vbo.buffer_data(nv, verts);
    vbo.unbind();

    ibo.bind();
    ibo.buffer_data(ne, elems);
    ibo.unbind();
}

void HumanoidMesh::step(float const dt, float const t) {
    legs.step(dt, t);
    arms.step(dt, t);
    arms.state = (HumanoidMesh::Arms::state_e)legs.state;
}

void HumanoidMesh::Legs::sync(Vt_classic *const verts, uint16_t *const vtop, uint32_t *const elems, uint16_t *const etop) const {
    uint16_t vbase = *vtop;
    elems[(*etop)++] = vbase + 0;
    elems[(*etop)++] = vbase + 2;
    elems[(*etop)++] = vbase + 1;
    elems[(*etop)++] = vbase + 0;
    elems[(*etop)++] = vbase + 2;
    elems[(*etop)++] = vbase + 3;
    elems[(*etop)++] = vbase + 4+0;
    elems[(*etop)++] = vbase + 4+2;
    elems[(*etop)++] = vbase + 4+1;
    elems[(*etop)++] = vbase + 4+0;
    elems[(*etop)++] = vbase + 4+2;
    elems[(*etop)++] = vbase + 4+3;

    #define LEG_OUTER (5.f/16.f)
    #define LEG_INNER (0.f/16.f)

    verts[(*vtop)++] = {{-LEG_OUTER, 0   ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{-LEG_OUTER, pos ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{-LEG_INNER, pos ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{-LEG_INNER, 0   ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{ LEG_INNER, 0   ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{ LEG_INNER, -pos,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{ LEG_OUTER, -pos,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{ LEG_OUTER, 0   ,   0.f}, {0.5,0.5}};

}

void HumanoidMesh::Legs::step(float const dt, float const t) {
    (void)dt;
    float amp, freq;
    switch (this->state) {
    case STOOD:
        this->target_pos -= this->target_pos * 0.1f;
        break;
    case WALKING:
        amp = 0.4; freq = 11.f;
        break;
    case RUNNING:
        amp = 0.55; freq = 16.f;
        break;
    }
    if (this->state != STOOD) {
        this->target_pos = amp * sin(freq * (t));
    }
    pos = pos + (this->target_pos - pos) * 0.66;
}

void HumanoidMesh::Torso::sync(Vt_classic *const verts, uint16_t *const vtop, uint32_t *const elems, uint16_t *const etop) const {
    uint16_t vbase = *vtop;
    elems[(*etop)++] = vbase + 0;
    elems[(*etop)++] = vbase + 2;
    elems[(*etop)++] = vbase + 1;
    elems[(*etop)++] = vbase + 0;
    elems[(*etop)++] = vbase + 2;
    elems[(*etop)++] = vbase + 3;

    #define TORSO_W (10.f/16.f)
    #define TORSO_H (4.f/16.f)

    verts[(*vtop)++] = {{-(TORSO_W/2.f), -(TORSO_H/2.f), 0.5f}, {0.5f+0.003,0.5f+0.003}};
    verts[(*vtop)++] = {{-(TORSO_W/2.f),  (TORSO_H/2.f), 0.5f}, {0.5f+0.003,0.5f+0.003}};
    verts[(*vtop)++] = {{ (TORSO_W/2.f),  (TORSO_H/2.f), 0.5f}, {0.5f+0.003,0.5f+0.003}};
    verts[(*vtop)++] = {{ (TORSO_W/2.f), -(TORSO_H/2.f), 0.5f}, {0.5f+0.003,0.5f+0.003}};
}

void HumanoidMesh::Arms::sync(Vt_classic *const verts, uint16_t *const vtop, uint32_t *const elems, uint16_t *const etop) const {
    uint16_t vbase = *vtop;
    elems[(*etop)++] = vbase + 0;
    elems[(*etop)++] = vbase + 2;
    elems[(*etop)++] = vbase + 1;
    elems[(*etop)++] = vbase + 0;
    elems[(*etop)++] = vbase + 2;
    elems[(*etop)++] = vbase + 3;
    elems[(*etop)++] = vbase + 4+0;
    elems[(*etop)++] = vbase + 4+2;
    elems[(*etop)++] = vbase + 4+1;
    elems[(*etop)++] = vbase + 4+0;
    elems[(*etop)++] = vbase + 4+2;
    elems[(*etop)++] = vbase + 4+3;
    elems[(*etop)++] = vbase + 8+0;
    elems[(*etop)++] = vbase + 8+2;
    elems[(*etop)++] = vbase + 8+1;
    elems[(*etop)++] = vbase + 8+0;
    elems[(*etop)++] = vbase + 8+2;
    elems[(*etop)++] = vbase + 8+3;
    elems[(*etop)++] = vbase + 12+0;
    elems[(*etop)++] = vbase + 12+2;
    elems[(*etop)++] = vbase + 12+1;
    elems[(*etop)++] = vbase + 12+0;
    elems[(*etop)++] = vbase + 12+2;
    elems[(*etop)++] = vbase + 12+3;

    #define ARM_W (3.f/16.f)
    #define ARM_H (TORSO_H)
    #define ARM_D (5.f/16.f)

    verts[(*vtop)++] = {{ -ARM_D,        0   ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{ -ARM_D,        pos ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{-(ARM_D+ARM_W), pos ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{-(ARM_D+ARM_W), 0   ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{  ARM_D,        0   ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{  ARM_D,        -pos,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{ (ARM_D+ARM_W), -pos,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{ (ARM_D+ARM_W), 0   ,   0.f}, {0.5,0.5}};

    verts[(*vtop)++] = {{ -ARM_D,        -((ARM_H/2.f)-(abs(pos)/5.f)),   0.1f}, {0.5,0.5}};
    verts[(*vtop)++] = {{ -ARM_D,         ((ARM_H/2.f)-(abs(pos)/5.f)),   0.1f}, {0.5,0.5}};
    verts[(*vtop)++] = {{-(ARM_D+ARM_W),  ((ARM_H/2.f)-(abs(pos)/5.f)),   0.1f}, {0.5,0.5}};
    verts[(*vtop)++] = {{-(ARM_D+ARM_W), -((ARM_H/2.f)-(abs(pos)/5.f)),   0.1f}, {0.5,0.5}};
    verts[(*vtop)++] = {{  ARM_D,         ((ARM_H/2.f)-(abs(pos)/5.f)),   0.1f}, {0.5,0.5}};
    verts[(*vtop)++] = {{  ARM_D,        -((ARM_H/2.f)-(abs(pos)/5.f)),   0.1f}, {0.5,0.5}};
    verts[(*vtop)++] = {{ (ARM_D+ARM_W), -((ARM_H/2.f)-(abs(pos)/5.f)),   0.1f}, {0.5,0.5}};
    verts[(*vtop)++] = {{ (ARM_D+ARM_W),  ((ARM_H/2.f)-(abs(pos)/5.f)),   0.1f}, {0.5,0.5}};

}

void HumanoidMesh::Arms::step(float const dt, float const t) {
    (void)dt;
    float amp, freq;
    switch (this->state) {
    case STOOD:
        this->target_pos -= this->target_pos * 0.1f;
        break;
    case WALKING:
        amp = 0.4; freq = 11.f;
        break;
    case RUNNING:
        amp = 0.45; freq = 16.f;
        break;
    case AIMING:
        break;
    }
    if (this->state != STOOD) {
        this->target_pos = amp * -sin(freq * (t));
    }
    pos = pos + (this->target_pos - pos) * 0.66;
}

void HumanoidMesh::Head::sync(Vt_classic *const verts, uint16_t *const vtop, uint32_t *const elems, uint16_t *const etop) const {
    uint16_t vbase = *vtop;
    elems[(*etop)++] = vbase + 0;
    elems[(*etop)++] = vbase + 2;
    elems[(*etop)++] = vbase + 1;
    elems[(*etop)++] = vbase + 0;
    elems[(*etop)++] = vbase + 2;
    elems[(*etop)++] = vbase + 3;

    #define HEAD_W (6.f/16.f)

    verts[(*vtop)++] = {{-(HEAD_W/2.f), -(HEAD_W/2.f), 0.6f}, {0.5f,0.5f}};
    verts[(*vtop)++] = {{-(HEAD_W/2.f),  (HEAD_W/2.f), 0.6f}, {0.5f,0.5f}};
    verts[(*vtop)++] = {{ (HEAD_W/2.f),  (HEAD_W/2.f), 0.6f}, {0.5f,0.5f}};
    verts[(*vtop)++] = {{ (HEAD_W/2.f), -(HEAD_W/2.f), 0.6f}, {0.5f,0.5f}};
}
