#include "Player.h"
#include <flgl/logger.h>
#include "game/components.h"
#include "game/Movement.h"
#include "game/Tiledefs.h"
#include "game/Tiles.h"
LOG_MODULE(plyr);
using namespace glm;

void PlayerActor::take_turn(entID self, State& state, Keyboard const& kb, world_mouse_t const& wm) {
    
    auto& pobj = state.world.getComp<c_Object>(self);
    if (kb[GLFW_KEY_W].down || kb[GLFW_KEY_A].down || kb[GLFW_KEY_S].down || kb[GLFW_KEY_D].down) {
        auto& move = state.world.addComp<c_Move>(self);
        move.clip_rad = 0.2f;
        if (kb[GLFW_KEY_W].down) move.v.y += (4.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
        if (kb[GLFW_KEY_A].down) move.v.x -= (4.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
        if (kb[GLFW_KEY_S].down) move.v.y -= (4.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
        if (kb[GLFW_KEY_D].down) move.v.x += (4.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
    }

    pobj.rot = vectorToAngle(wm.pos - pobj.pos);

    if (wm.mouse->left.down) {
        sTiles::destroy_clear(state.world, wm.pos);
        sTiles::wall.place(state.world, &state.world.tile_at(wm.pos), {0,0});
        LOG_INF("placed %p, readback props %d", &state.world.tile_at(wm.pos), state.world.tile_at(wm.pos).surf.props.val);
    }
}

Player Player::spawn(World *const world, glm::vec2 pos) {
    entID e = world->newEntity();
    auto& pobj = world->addComp<c_Object>(e);
    pobj.pos = pos;
    pobj.rot = 0.f;
    pobj.scale = vec2(1.f);
    pobj.anc = vec2(0.f);
    world->addComp<c_StaticAtlas>(e) = c_StaticAtlas::from_sheet({63.f + 3.f/16.f, 3.f/16.f}, vec2(10.f/16.f));
    c_Actor& ator = world->addComp<c_Actor>(e);
    ator.emplace<PlayerActor>();
    return Player{e};
}

void Player::despawn(World *const world, Player player) {
    world->removeEntity(player.id);
}
