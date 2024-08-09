#include "Actor.h"
#include <flgl/logger.h>
LOG_MODULE(actor);

void c_Actor::take_all_turns(State& state, Keyboard const& kb, world_mouse_t const& wm) {
    for (auto actor_e : state.world.ecs.view<c_Actor>()) {
        Actor& actor = state.world.ecs.getComp<c_Actor>(actor_e).get();
        actor.take_turn(actor_e, state, kb, wm);
    }
}

