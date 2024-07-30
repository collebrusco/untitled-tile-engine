#include "Actor.h"

Actor& c_Actor::get() {
    return (*((Actor*)data));
}


void c_Actor::destroy() {
    this->get().~Actor();
}

void c_Actor::step(ECS* ecs, ActionPool* apool) {
    for (auto actor_e : ecs->view<c_Actor>()) {
        Actor& actor = ecs->getComp<c_Actor>(actor_e).get();
        actor.take_turn(apool);
    }
}

