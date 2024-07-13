/** 
 * Engine.h
 * created 06/22/24 by frank collebrusco 
 * The engine should only contain mechanisms for updating and running the 'in game'
 * The game state is represented in State.h, no global game 
 */
#ifndef ENGINE_H
#define ENGINE_H
#include <flgl.h>
#include <flgl/glm.h>
#include "Stopwatch.h"
#include "game/State.h"
#include "game/System.h"

struct Engine {
    SystemList syslist;
    Stopwatch timer;
    void init(State* state);
    void step(float dt, State* state, Keyboard const& kb, Mouse const& mouse, glm::vec2 wmpos, glm::vec2 wmdelt);
    void destroy(State* state);
};

#endif /* ENGINE_H */
