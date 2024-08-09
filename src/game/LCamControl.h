/** 
 * LCamControl.h
 * created 07/21/24 by frank collebrusco 
 */
#ifndef LCAM_CONTROL_H
#define LCAM_CONTROL_H
#include "render_pipeline_structs.h"
#include "State.h"

class LCamControl {
    float speed{1.f};
    void scroll(local_cam_t& lcam, float dt);
public:
    static void spawn(State& state);
    static void follow(State& state, entID tar, float pct);
    static void nofollow(State& state);
    void set_speed(float sens);
    void update(local_cam_t& lcam, entID e, World& world, float dt);
};



#endif /* LCAM_CONTROL_H */
