/** 
 * State.h
 * created 06/22/24 by frank collebrusco 
 */
#ifndef STATE_H
#define STATE_H
#include "game/World.h"
#include "render_pipeline_structs.h"

struct State {
    State();

    World world;
    local_cam_t lcam;

    /**
     * TODO: more things like money, player data, etc. anything in game 
     */
};

#endif /* STATE_H */
