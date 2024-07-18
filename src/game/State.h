/** 
 * State.h
 * created 06/22/24 by frank collebrusco 
 */
#ifndef STATE_H
#define STATE_H
#include "game/World.h"
#include "render_pipeline_structs.h"

/*****
 * this is kinda stupid 
 * state is kinda tied to systems so why not let them own it and include e/o's headers?
 * have i reinvented oop..
 * 
 * this is opposed to class System {
 *  virtual init, up(dt), dest
 *  // getters for needed data for other sys's...
 *  // no other controls?
 *  // then how do they get eos instances <-----
 * only one sys instance per game anyway 
 * }
 */

struct State {
    State();

    World world;
    local_cam_t lcam;

    /**
     * TODO: more things like money, player data, etc. anything in game 
     */
};

#endif /* STATE_H */
