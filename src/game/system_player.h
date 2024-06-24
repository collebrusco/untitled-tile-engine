/** 
 * system_player.h
 * created 06/23/24 by frank collebrusco 
 */
#ifndef SYSTEM_PLAYER_H
#define SYSTEM_PLAYER_H
#include "game/State.h"
#include "game/Engine.h"
#include "game/System.h"

void player_system_init(State* state);
entID player_system_get_player();
void player_system_step(float dt, State* state, Keyboard const& kb, Mouse const& mouse, glm::vec2 wmpos, glm::vec2 wmdelt);

#define PLAYER_SYSTEM_STRUCT ((system_t){.init = player_system_init, .step = player_system_step, .dest = 0})

#endif /* SYSTEM_PLAYER_H */
