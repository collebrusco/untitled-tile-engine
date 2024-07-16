#include "Engine.h"
#include <flgl/logger.h>
LOG_MODULE(eng);
using namespace glm;

void Engine::init(State* state) {
    timer.reset_start();
    for (system_t& system : syslist) {
        if (system.init) {
            system.init(state);
        }
    }
}

void Engine::step(float dt, State* state, Keyboard const& kb, Mouse const& mouse, vec2 wmpos, vec2 wmdelt) {
    for (system_t& system : syslist) {
        if (system.step && (timer.read(SECONDS) - system.last_time_s) > system.period_s) {
            system.step(dt, state, kb, mouse, wmpos, wmdelt);
            system.last_time_s = timer.read(SECONDS);
        }
    }
}

void Engine::destroy(State* state) {
    for (system_t& system : syslist) {
        if (system.dest) system.dest(state);
    }
}
