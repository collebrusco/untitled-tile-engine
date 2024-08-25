#include "HumanoidMesh.h"
#include "util/math.h"
#include <flgl/logger.h>
LOG_MODULE(hmesh);
using namespace glm;

/** 
 * the mesh animations are kinda hacked and should be in a vertex shader or something.
 * the renderer buffers the whole mesh for each draw here which is probly bad
 * i just want it to be one draw call other than buffering model mat for each part
 */

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

void HumanoidMesh::step(c_Object const& obj, float const dt, float const t) {
    legs.step(dt, t);
    arms.step(dt, t);
    head.step(obj.rot, torso.rot.val);
    torso.step(dt, obj.rot);
    if (arms.state != Arms::AIMING) arms.state = (HumanoidMesh::Arms::state_e)legs.state;
}

glm::mat4 HumanoidMesh::model(c_Object const &obj) const {
    return genModelMat2d(obj.pos, torso.rot.val, obj.scale, obj.anc);
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

    verts[(*vtop)++] = {{-LEG_OUTER, 0       ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{-LEG_OUTER, pos.val ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{-LEG_INNER, pos.val ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{-LEG_INNER, 0       ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{ LEG_INNER, 0       ,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{ LEG_INNER, -pos.val,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{ LEG_OUTER, -pos.val,   0.f}, {0.5,0.5}};
    verts[(*vtop)++] = {{ LEG_OUTER, 0       ,   0.f}, {0.5,0.5}};
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

void HumanoidMesh::Torso::step(float const dt, float const tar) {
    rot.tar = tar;
    rot.step(0.20f, 0.015, dt);
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
    static const vec2 l_arm_c = vec2((ARM_W/2.f)-ARM_D, 0.f);
    static const vec2 r_arm_c = vec2((ARM_W/2.f)+ARM_D, 0.f);

    verts[(*vtop)++] = {rotate_around_pt({ -ARM_D,         0                              ,    0.f}, l_arm_c, angle.val.x), {0.5,0.5}};
    verts[(*vtop)++] = {rotate_around_pt({ -ARM_D,         length.val.x                   ,    0.f}, l_arm_c, angle.val.x), {0.5,0.5}};
    verts[(*vtop)++] = {rotate_around_pt({-(ARM_D+ARM_W),  length.val.x                   ,    0.f}, l_arm_c, angle.val.x), {0.5,0.5}};
    verts[(*vtop)++] = {rotate_around_pt({-(ARM_D+ARM_W),  0                              ,    0.f}, l_arm_c, angle.val.x), {0.5,0.5}};
    verts[(*vtop)++] = {rotate_around_pt({  ARM_D,         0                              ,    0.f}, r_arm_c, angle.val.y), {0.5,0.5}};
    verts[(*vtop)++] = {rotate_around_pt({  ARM_D,         length.val.y                   ,    0.f}, r_arm_c, angle.val.y), {0.5,0.5}};
    verts[(*vtop)++] = {rotate_around_pt({ (ARM_D+ARM_W),  length.val.y                   ,    0.f}, r_arm_c, angle.val.y), {0.5,0.5}};
    verts[(*vtop)++] = {rotate_around_pt({ (ARM_D+ARM_W),  0                              ,    0.f}, r_arm_c, angle.val.y), {0.5,0.5}};

    verts[(*vtop)++] = {rotate_around_pt({ -ARM_D,        -((ARM_H/2.f)-(abs(length.val.x)/5.f)),   0.1f}, l_arm_c, angle.val.x), {0.5,0.5}};
    verts[(*vtop)++] = {rotate_around_pt({ -ARM_D,         ((ARM_H/2.f)-(abs(length.val.x)/5.f)),   0.1f}, l_arm_c, angle.val.x), {0.5,0.5}};
    verts[(*vtop)++] = {rotate_around_pt({-(ARM_D+ARM_W),  ((ARM_H/2.f)-(abs(length.val.x)/5.f)),   0.1f}, l_arm_c, angle.val.x), {0.5,0.5}};
    verts[(*vtop)++] = {rotate_around_pt({-(ARM_D+ARM_W), -((ARM_H/2.f)-(abs(length.val.x)/5.f)),   0.1f}, l_arm_c, angle.val.x), {0.5,0.5}};
    verts[(*vtop)++] = {rotate_around_pt({  ARM_D,         ((ARM_H/2.f)-(abs(length.val.y)/5.f)),   0.1f}, r_arm_c, angle.val.y), {0.5,0.5}};
    verts[(*vtop)++] = {rotate_around_pt({  ARM_D,        -((ARM_H/2.f)-(abs(length.val.y)/5.f)),   0.1f}, r_arm_c, angle.val.y), {0.5,0.5}};
    verts[(*vtop)++] = {rotate_around_pt({ (ARM_D+ARM_W), -((ARM_H/2.f)-(abs(length.val.y)/5.f)),   0.1f}, r_arm_c, angle.val.y), {0.5,0.5}};
    verts[(*vtop)++] = {rotate_around_pt({ (ARM_D+ARM_W),  ((ARM_H/2.f)-(abs(length.val.y)/5.f)),   0.1f}, r_arm_c, angle.val.y), {0.5,0.5}};

}

void HumanoidMesh::Arms::step(float const dt, float const t) {
    float amp, freq, Kp, Ki;
    Kp = 0.25f, Ki = 0.025f;
    switch (this->state) {
    case STOOD:
        length.tar.x = length.tar.y = angle.tar.y = angle.tar.x = 0.f;
        break;
    case WALKING:
        amp = 0.2; freq = 11.f;
        break;
    case RUNNING:
        amp = 0.25; freq = 16.f;
        break;
    case AIMING:
        length.tar.x = length.tar.y = 0.5f;
        angle.tar.y = 15.f;
        angle.tar.x = -50.f;
        break;
    case SWINGING0:
        gent = 0.f;
        break;
    case SWINGING1:
        break;
    case SWINGING2:
        break;
    }
    if (this->state == WALKING || this->state == RUNNING) {
        length.tar.x = amp * -sin(freq * (t));
        length.tar.y = -length.tar.x;
        angle.tar.y = angle.tar.x = 0.f;
    }
    length.step(Kp, Ki, dt); angle.step(Kp, Ki, dt);
}

void HumanoidMesh::Legs::step(float const dt, float const t) {
    (void)dt;
    float amp, freq;
    switch (this->state) {
    case STOOD:
        this->pos.tar -= this->pos.tar * 0.1f;
        break;
    case WALKING:
        amp = 0.35; freq = 11.f;
        break;
    case RUNNING:
        amp = 0.45; freq = 16.f;
        break;
    }
    if (this->state != STOOD) {
        this->pos.tar = amp * sin(freq * (t));
    }
    float Kp = 0.2f, Ki = 0.02f;
    pos.step(Kp, Ki, dt);
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

    glm::vec3 a = rotate_around_origin(glm::vec3(-(HEAD_W/2.f), -(HEAD_W/2.f), 0.6f), angle);
    glm::vec3 b = rotate_around_origin(glm::vec3(-(HEAD_W/2.f),  (HEAD_W/2.f), 0.6f), angle);
    glm::vec3 c = rotate_around_origin(glm::vec3( (HEAD_W/2.f),  (HEAD_W/2.f), 0.6f), angle);
    glm::vec3 d = rotate_around_origin(glm::vec3( (HEAD_W/2.f), -(HEAD_W/2.f), 0.6f), angle);

    verts[(*vtop)++] = {a, {0.5f,0.5f}};
    verts[(*vtop)++] = {b, {0.5f,0.5f}};
    verts[(*vtop)++] = {c, {0.5f,0.5f}};
    verts[(*vtop)++] = {d, {0.5f,0.5f}};
}

void HumanoidMesh::Head::step(float ha, float ta) {
    angle = ha - ta;
}

