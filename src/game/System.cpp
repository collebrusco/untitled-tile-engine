#include "System.h"
#include <flgl/logger.h>
LOG_MODULE(sys);

System::System(float pd) : _pd(pd), _t(0.f) {}

void System::init(State* state) {
    intr_init(state);
}

void System::destroy(State* state) {
    intr_destroy(state);
}

void System::step(float dt, State* state, Keyboard const& kb, Mouse const& mouse, glm::vec2 wmpos, glm::vec2 wmdelt, float t_s) {
    if ((t_s - _t) > _pd) {
        this->intr_step(dt, state, kb, mouse, wmpos, wmdelt, t_s);
        _t = t_s;
    }
}

