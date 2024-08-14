#include "Player.h"
#include <flgl/logger.h>
#include "data/components.h"
#include "game/Movement.h"
#include "game/Tiledefs.h"
#include "game/Tiles.h"
#include "game/Entity.h"
#include "data/Animation.h"
LOG_MODULE(plyr);
using namespace glm;

void PlayerActor::take_turn(entID self, State& state, Keyboard const& kb, world_mouse_t const& wm) {
    auto& A = kb[GLFW_KEY_A]; auto& W = kb[GLFW_KEY_W]; auto& S = kb[GLFW_KEY_S]; auto& D = kb[GLFW_KEY_D]; 
    auto& shift = kb[GLFW_KEY_LEFT_SHIFT];
    auto& pobj = state.world.getComp<c_Object>(self);
    bool movekey = A.down || S.down || D.down || W.down;
    if (movekey) {
        auto& move = state.world.addComp<c_Move>(self);
        move.clip_rad = 0.2f;
        move.props.friction = CMOVE_FRICTION_FULL;
        float speed = !shift.down ? 2.f : 4.f;
        if (shift.down) {
            /** this needs to go elsewhere (maybe animcontrol comp?) 
             * because it needs to be optional and reusable 
             * (and isnt really related to 'taking ur turn') 
             * really think this needs to be event maybe? 
             * */
            Entity::set_anim_if_not(self, state.world, &Animations::character_run);
            pobj.rot = A.down ? 90.f : 270.f;
        } else {
            Entity::set_anim_if_not(self, state.world, &Animations::character_walk);
        }
        if (W.down) move.v.y += speed;
        if (A.down) move.v.x -= speed;
        if (S.down) move.v.y -= speed;
        if (D.down) move.v.x += speed;
    } else if (W.released || A.released || S.released || D.released) {
        Entity::set_anim_if_not(self, state.world, &Animations::character_stand);
    }

    if (!(movekey && shift.down)) pobj.rot = vectorToAngle(wm.pos - pobj.pos);

    if (wm.mouse->left.down) {
        sTiles::destroy_clear(state.world, wm.pos);
        sTiles::wall.place(state.world, &state.world.tile_at(wm.pos), {0,0});
    }
}

Player Player::spawn(World *const world, glm::vec2 pos) {
    entID e = world->newEntity();
    auto& pobj = world->addComp<c_Object>(e);
    pobj.pos = pos;
    pobj.rot = 0.f;
    // pobj.scale = vec2(1.f);
    pobj.anc = vec2(1.f/32.f, 0.2f);
    // world->addComp<c_EntRenderEntry>(e) = c_EntRenderEntry::from_sheet({63.f + 3.f/16.f, 3.f/16.f}, vec2(10.f/16.f));
    Entity::config_for_sprite(e, *world, Sprites::char_0);
    c_Actor& ator = world->addComp<c_Actor>(e);
    ator.emplace<PlayerActor>();

    Entity::set_anim_if_not(e, *world, &Animations::character_stand);

    return Player{e};
}

void Player::despawn(World *const world, Player player) {
    world->removeEntity(player.id);
}
