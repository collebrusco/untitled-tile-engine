#include "Engine.h"
#include <flgl/logger.h>
LOG_MODULE(eng);
using namespace glm;

void Engine::init(State* state) {
    for (system_t system : syslist) {
        if (system.init) {
            system.init(state);
        }
    }
}

void Engine::step(float dt, State* state, Keyboard const& kb, Mouse const& mouse, vec2 wmpos, vec2 wmdelt) {
    for (system_t system : syslist) {
        if (system.step) system.step(dt, state, kb, mouse, wmpos, wmdelt);
    }
}

void Engine::destroy(State* state) {
    for (system_t system : syslist) {
        if (system.dest) system.dest(state);
    }
}
