#include "Actor.h"
#include <flgl/logger.h>
LOG_MODULE(actor);

void c_Actor::take_all_turns(ECS* ecs, ActionPool* apool) {
    for (auto actor_e : ecs->view<c_Actor>()) {
        Actor& actor = ecs->getComp<c_Actor>(actor_e).get();
        actor.take_turn(apool);
    }
}

