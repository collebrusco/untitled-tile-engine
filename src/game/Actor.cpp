#include "Actor.h"
#include <flgl/logger.h>
LOG_MODULE(actor);

Actor& c_Actor::get() {
    return (*((Actor*)data));
}


void c_Actor::destroy() {
    this->get().~Actor();
}

void c_Actor::take_all_turns(ECS* ecs, ActionPool* apool) {
    for (auto actor_e : ecs->view<c_Actor>()) {
        Actor& actor = ecs->getComp<c_Actor>(actor_e).get();
        actor.take_turn(apool);
    }
}

