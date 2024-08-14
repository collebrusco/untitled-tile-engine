/** 
 * State.h
 * created 06/22/24 by frank collebrusco 
 */
#ifndef STATE_H
#define STATE_H
#include "data/World.h"
#include "game/Action.h"
#include "data/render_pipeline_structs.h"

struct State {
    State();

    World world;
    struct {
        local_cam_t lcam;
        entID e;
    } cam;

    ActionPool actions;

    /**
     * TODO: more things like money, player data, etc. anything in game 
     */
};

#endif /* STATE_H */
