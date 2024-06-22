#include "NWOcam.h"
using namespace glm;

NWOcam::NWOcam(frame_manager_t * fmngr) : frame_manager(fmngr) {
}

bool NWOcam::update_condition() const {
    return shouldUpdate() || frame_manager->get_fbuff_wh_region() != prev_frame;
}

mat4 const& NWOcam::updateProj() {
    setShouldUpdate();
    vec2 orthoDims = (vec2)(frame_manager->get_fbuff_wh_tile());
    vec4 lrbt = (lrbtMat * orthoDims);
    _proj = ortho(lrbt.x, lrbt.y, lrbt.z, lrbt.w, near, far);
    return _proj;
}

void NWOcam::update() {
    this->Camera::update();
    prev_frame = frame_manager->get_fbuff_wh_region();
}

