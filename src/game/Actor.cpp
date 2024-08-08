#include "Actor.h"
#include <flgl/logger.h>
LOG_MODULE(actor);

void c_Actor::take_all_turns(World*const world, ActionPool* const apool) {
    for (auto actor_e : world->ecs.view<c_Actor>()) {
        Actor& actor = world->ecs.getComp<c_Actor>(actor_e).get();
        actor.take_turn(actor_e, world, apool);
    }
}

