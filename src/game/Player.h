/** 
 * Player.h
 * created 08/07/24 by frank collebrusco 
 */
#ifndef PLAYER_H
#define PLAYER_H
#include "ECS.h"
#include "game/World.h"
#include "game/Actor.h"

struct PlayerActor final : public Actor {
    virtual ~PlayerActor() = default;
    virtual void take_turn(entID self, World*const world, ActionPool* const apool) override final;
};

class Player {
public:
    static entID spawn(World* const world, glm::vec2 pos);
};

#endif /* PLAYER_H */
