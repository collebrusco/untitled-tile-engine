/** 
 * system_actor.h
 * created 07/14/24 by frank collebrusco 
 */
#ifndef SYSTEM_ACTOR_H
#define SYSTEM_ACTOR_H
#include "game/State.h"

#define MAX_ACTOR_SIZE (64)

// class Actor {
// public:
//     virtual void perform(float dt) = 0;
// };

// struct ActorBuffer {
//     alignas(Actor) char mem[MAX_ACTOR_SIZE];
//     void allocActor() {
//         Actor* actor = new (this->mem) ActorEg();
//     }
//     Actor* getActor() {
//         return (Actor*) this->mem;
//     }
// };

// class ActorEg : public Actor {
// public:
//     virtual void perform(float dt) override final {}
// };
// static_assert(sizeof(ActorEg) <= MAX_ACTOR_SIZE);


void actor_system_init(State* state);
void actor_system_dest(State* state);
void actor_system_step(float dt, State* state, Keyboard const& kb, Mouse const& mouse, glm::vec2 wmpos, glm::vec2 wmdelt);


#endif /* SYSTEM_ACTOR_H */
