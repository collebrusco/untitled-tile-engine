#include "system_player.h"
#include "components.h"
#include "ECS.h"
#include <flgl/logger.h>
#include <flgl/tools.h>
LOG_MODULE(plyr);
using namespace glm;

#define WRLD state->world
#define ESYS state->world.ecs

PlayerSystem::PlayerSystem() : System(0.f)
{
}

void PlayerSystem::intr_init(State *state)
{
    player = ESYS.newEntity();
    auto& pobj = ESYS.addComp<c_Object>(player);
    pobj.pos = vec2(0.f);
    pobj.rot = 0.f;
    pobj.scale = vec2(1.f);
    pobj.anc = vec2(0.f);
    ESYS.addComp<c_StaticAtlas>(player) = c_StaticAtlas::from_sheet({63.f + 3.f/16.f, 3.f/16.f}, vec2(10.f/16.f));
}

void PlayerSystem::intr_destroy(State *state)
{
}

void PlayerSystem::intr_step(float dt, State *state, Keyboard const &kb, Mouse const &mouse, glm::vec2 wmpos, glm::vec2 wmdelt, float t)
{
    auto& pobj = ESYS.getComp<c_Object>(player);
    if (kb[GLFW_KEY_W].down) pobj.pos.y += dt * (4.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	if (kb[GLFW_KEY_A].down) pobj.pos.x -= dt * (4.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	if (kb[GLFW_KEY_S].down) pobj.pos.y -= dt * (4.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	if (kb[GLFW_KEY_D].down) pobj.pos.x += dt * (4.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));

    // LOG_INF("a: %.0f", vectorToAngle(wmpos - pobj.pos));

    pobj.rot = vectorToAngle(wmpos - pobj.pos);
}
