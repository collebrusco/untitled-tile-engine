#include "Player.h"
#include <flgl/logger.h>
#include "game/components.h"
#include "game/Movement.h"
LOG_MODULE(plyr);
using namespace glm;

void PlayerActor::take_turn(entID self, World*const world, ActionPool *const apool) {
    (void)apool;
    c_Move* mov = world->ecs.tryGetComp<c_Move>(self);
    if (mov) {
        world->ecs.removeComp<c_Move>(self);
    }
    mov = &world->ecs.addComp<c_Move>(self);
    mov->clip_rad = 0.5f;
    mov->v = {0.f, 0.2f};
}

entID Player::spawn(World *const world, glm::vec2 pos) {
    ECS& ecs = world->ecs;
    entID e = ecs.newEntity();
    auto& pobj = ecs.addComp<c_Object>(e);
    pobj.pos = pos;
    pobj.rot = 0.f;
    pobj.scale = vec2(1.f);
    pobj.anc = vec2(0.f);
    ecs.addComp<c_StaticAtlas>(e) = c_StaticAtlas::from_sheet({63.f + 3.f/16.f, 3.f/16.f}, vec2(10.f/16.f));
    c_Actor& ator = ecs.addComp<c_Actor>(e);
    ator.emplace<PlayerActor>();
    return e;
}
