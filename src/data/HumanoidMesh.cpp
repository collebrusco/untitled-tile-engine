#include "HumanoidMesh.h"


void HumanoidMesh::sync(VertexBuffer<Vt_classic> &vbo, ElementBuffer &ibo) const {
    const uint16_t nv = HumanoidLegs::num_verts + HumanoidTorso::num_verts + HumanoidArms::num_verts;
    const uint16_t ne = HumanoidLegs::num_elems + HumanoidTorso::num_elems + HumanoidArms::num_elems;
    Vt_classic verts[nv];
    uint32_t elems[ne];
    uint16_t vt = 0, et = 0;
    legs.sync(verts, &vt, elems, &et);
    torso.sync(verts, &vt, elems, &et);
    arms.sync(verts, &vt, elems, &et);


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
    arms.state = (HumanoidArms::state_e)legs.state;
}

void HumanoidLegs::sync(Vt_classic *const verts, uint16_t *const vtop, uint32_t *const elems, uint16_t *const etop) const {
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

    verts[(*vtop)++] = {{-0.4, 0   ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{-0.4, pos ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{0   , pos ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{0   , 0   ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{0   , 0   ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{0   , -pos,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{0.4 , -pos,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{0.4 , 0   ,   0.f}, {0.5,0.5}};

}

void HumanoidLegs::step(float const dt, float const t) {
    (void)dt;
    float amp, freq;
    switch (this->state) {
    case STOOD:
        this->target_pos -= this->target_pos * 0.2f;
        break;
    case WALKING:
        amp = 0.4; freq = 12.f;
        break;
    case RUNNING:
        amp = 0.55; freq = 16.f;
        break;
    }
    if (this->state != STOOD) {
        this->target_pos = amp * sin(freq * (t));
    }
    pos = pos + (this->target_pos - pos) * 0.8;
}

void HumanoidTorso::sync(Vt_classic *const verts, uint16_t *const vtop, uint32_t *const elems, uint16_t *const etop) const {
    uint16_t vbase = *vtop;
    elems[(*etop)++] = vbase + 0;
    elems[(*etop)++] = vbase + 2;
    elems[(*etop)++] = vbase + 1;
    elems[(*etop)++] = vbase + 0;
    elems[(*etop)++] = vbase + 2;
    elems[(*etop)++] = vbase + 3;

    #define TORSO_W (0.45f)
    #define TORSO_H (2.5f/16.f)

    verts[(*vtop)++] = {{-TORSO_W, -TORSO_H, 0.5f}, {0.5f+0.003,0.5f+0.003}};
    verts[(*vtop)++] = {{-TORSO_W,  TORSO_H, 0.5f}, {0.5f+0.003,0.5f+0.003}};
    verts[(*vtop)++] = {{ TORSO_W,  TORSO_H, 0.5f}, {0.5f+0.003,0.5f+0.003}};
    verts[(*vtop)++] = {{ TORSO_W, -TORSO_H, 0.5f}, {0.5f+0.003,0.5f+0.003}};
}

void HumanoidArms::sync(Vt_classic *const verts, uint16_t *const vtop, uint32_t *const elems, uint16_t *const etop) const {
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

    verts[(*vtop)++] = {{-0.4, 0   ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{-0.4, pos ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{-0.6, pos ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{-0.6, 0   ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{ 0.4, 0   ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{ 0.4, -pos,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{0.6 , -pos,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{0.6 , 0   ,   0.f}, {0.5,0.5}};
}

void HumanoidArms::step(float const dt, float const t) {
    (void)dt;
    float amp, freq;
    switch (this->state) {
    case STOOD:
        this->target_pos -= this->target_pos * 0.2f;
        break;
    case WALKING:
        amp = 0.4; freq = 12.f;
        break;
    case RUNNING:
        amp = 0.55; freq = 16.f;
        break;
    case AIMING:
        break;
    }
    if (this->state != STOOD) {
        this->target_pos = amp * -sin(freq * (t));
    }
    pos = pos + (this->target_pos - pos) * 0.8;
}
