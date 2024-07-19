#include "System.h"
#include <flgl/logger.h>
LOG_MODULE(sys);

SystemList::SystemList() {
    memset(systems, 0x00, MAX_SYSTEMS * sizeof(system_t));
}

system_t& SystemList::next_system() {
    return systems[top++];
}

SystemList::SystemIt::SystemIt(SystemList& l, bool d) : list(l) {
    done = i = d;
}

SystemList::SystemIt& SystemList::SystemIt::operator++() {
    done = i == (MAX_SYSTEMS-1);
    i++;
    return *this;
}

SystemList::SystemIt SystemList::begin() {
    return SystemIt(*this);
}

SystemList::SystemIt SystemList::end() {
    return SystemIt(*this, 1);
}






System::System(float pd) : _pd(pd), _t(0.f) {}

void System::init(State* state) {
    intr_init(state);
}

void System::destroy(State* state) {
    intr_destroy(state);
}

void System::step(float dt, State* state, Keyboard const& kb, Mouse const& mouse, glm::vec2 wmpos, glm::vec2 wmdelt) {
    
}

