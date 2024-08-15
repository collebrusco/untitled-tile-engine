#include "MeshAnimStack.h"
#include <flgl/logger.h>
LOG_MODULE(as);

void AbsAnimStackLayer::add_quad_idxs(uint32_t* elems, uint16_t* etop) {
    elems[(*etop)++] = 0; elems[(*etop)++] = 2; elems[(*etop)++] = 1;
    elems[(*etop)++] = 0; elems[(*etop)++] = 2; elems[(*etop)++] = 3;
}


bool HandAnimStackLayer::step(float dt) {
    tmr += dt;
    if (tmr > frames[idx].t) {
        tmr = 0.f; 
        idx = (idx+1)%N;
        return true;
    }
    return false;
}

void HandAnimStackLayer::add_mesh(Vt_classic *verts, uint16_t *vtop, uint32_t *elems, uint16_t *etop, float z) const {
    {

    }
}

bool c_AnimationStack::exec(float dt, Vt_classic *verts, uint16_t *vtop, uint32_t *elems, uint16_t *etop) {
    bool res = false;
    for (AbsAnimStackLayer* layer : *this) {
        res = res || layer->step(dt);
    }
    if (res) {
        float z = 0.f;
        for (AbsAnimStackLayer* layer : *this) {
            layer->add_mesh(verts, vtop, elems, etop, z);
            z += 0.1;
        }
    }
    return res;
}

