/** 
 * Player.h
 * created 08/07/24 by frank collebrusco 
 */
#ifndef PLAYER_H
#define PLAYER_H
#include "ECS.h"
#include "data/World.h"
#include "game/Actor.h"

struct PlayerActor final : public Actor {
    virtual ~PlayerActor() = default;
    virtual void take_turn(entID self, State& state, Keyboard const& kb, world_mouse_t const& wm) override final;
};

struct Player {
    entID id;
    static Player spawn(World* const world, glm::vec2 pos);
    static void despawn(World* const world, Player player);
};

#endif /* PLAYER_H */
