#include "Engine.h"
#include <flgl/logger.h>
LOG_MODULE(eng);
using namespace glm;

Engine::Engine() : systems(SYSLIST_SIZE_BYTES) {}

void Engine::init(State* state) {
    timer.reset_start();
    for (System* sys : systems) {
        sys->init(state);
    }
}

void Engine::step(float dt, State* state, Keyboard const& kb, Mouse const& mouse, vec2 wmpos, vec2 wmdelt) {
    for (System* sys : systems) {
        sys->step(dt, state, kb, mouse, wmpos, wmdelt, timer.read());
    }
}

void Engine::destroy(State* state) {
    for (System* sys : systems) {
        sys->destroy(state);
    }
}
