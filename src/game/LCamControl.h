/** 
 * LCamControl.h
 * created 07/21/24 by frank collebrusco 
 */
#ifndef LCAM_CONTROL_H
#define LCAM_CONTROL_H
#include "render_pipeline_structs.h"

class LCamControl {
    float speed{1.f};
    void scroll(local_cam_t& lcam, float dt);
public:
    void set_speed(float sens);
    void update(local_cam_t& lcam, float dt);
};



#endif /* LCAM_CONTROL_H */
