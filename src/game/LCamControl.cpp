#include "LCamControl.h"
#include <flgl.h>
#include <flgl/glm.h>
#include <flgl/logger.h>
#include "constants.h"
LOG_MODULE(lcamc);

void LCamControl::set_speed(float sens) {
    this->speed = sens;
}

void LCamControl::scroll(local_cam_t& lcam, float dt) {
    if (window.mouse.scroll.y > 0.3) {
        lcam.frame.x += speed * dt * 33.f;
    }
    if (window.mouse.scroll.y < -0.3) {
        lcam.frame.x -= speed * dt * 33.f;
    }
    lcam.frame.x = glm::clamp(lcam.frame.x, 2.f, REGION_SIZE_F * WORLD_DIAMETER - REGION_SIZE_F/2.f);
}

void LCamControl::update(local_cam_t& lcam, float dt) {
    scroll(lcam, dt);
    local_cam_upf(lcam);
}

