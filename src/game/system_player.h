/** 
 * system_player.h
 * created 06/23/24 by frank collebrusco 
 */
#ifndef SYSTEM_PLAYER_H
#define SYSTEM_PLAYER_H
#include "game/State.h"
#include "game/Engine.h"
#include "game/System.h"

class PlayerSystem : public System {
public:
    entID player;
    PlayerSystem();
private:
    virtual void intr_init(State* state) override final;
    virtual void intr_destroy(State* state) override final;
    virtual void intr_step(float dt, State* state, Keyboard const& kb, Mouse const& mouse, glm::vec2 wmpos, glm::vec2 wmdelt, float t) override final;
};

#endif /* SYSTEM_PLAYER_H */
