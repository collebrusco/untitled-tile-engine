#include "system_player.h"
#include "components.h"
#include "ECS.h"
#include <flgl/logger.h>
LOG_MODULE(plyr);
using namespace glm;

static entID player = ~0;

#define WRLD state->world
#define ESYS state->world.ecs

void player_spawn(State* state, vec2) {
    LOG_INF("spawnin player");
    player = ESYS.newEntity();
    ESYS.addComp<c_Object>(player) = {
        .pos = vec2(0.f),
        .rot = 0.f,
        .scale = vec2(1.f),
        .anc = vec2(0.f)
    };
    ESYS.addComp<c_StaticAtlas>(player) = c_StaticAtlas::from_sheet({63.f + 3.f/16.f, 3.f/16.f}, vec2(10.f/16.f));
}

void player_system_init(State* state) {
    player_spawn(state, vec2(0.f));
}

void player_system_step(float dt, State* state, Keyboard const& kb, Mouse const& mouse, glm::vec2 wmpos, glm::vec2 wmdelt) {
    ESYS.getComp<c_Object>(player).rot += 0.2;
}


