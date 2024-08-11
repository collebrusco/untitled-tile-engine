#include "LCamControl.h"
#include <flgl.h>
#include <flgl/glm.h>
#include <flgl/logger.h>
#include "constants.h"
#include "game/components.h"
#include "game/Followers.h"
LOG_MODULE(lcamc);

void LCamControl::spawn(State &state) {
    state.cam.e = state.world.newEntity();
    state.world.addComp<c_Object>(state.cam.e).pos = state.cam.lcam.pos;
}

void LCamControl::follow(State &state, entID tar, float pct) {
    if (!state.world.entityValid(state.cam.e)) LOG_ERR("cam cant follow without spawning as entity");
    state.world.addComp<c_DiffFollower>(state.cam.e) = {.tar = tar, .pct = pct};
}

void LCamControl::nofollow(State &state) {
    if (!state.world.entityValid(state.cam.e)) LOG_ERR("cam cant unfollow without spawning as entity");
    state.world.removeComp<c_DiffFollower>(state.cam.e);
}

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

void LCamControl::update(local_cam_t& lcam, entID e, World& world, float dt) {
    if (world.entityValid(e)) {
        c_Object* co = world.tryGetComp<c_Object>(e);
        if (co) lcam.pos = co->pos;
    }
    world.relocate(lcam.pos);
    scroll(lcam, dt);
    local_cam_upf(lcam);
}

