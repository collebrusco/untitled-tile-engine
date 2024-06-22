// 
// NWOcam.h
// non-window ortho cam
// created 06/16/24 by frank collebrusco 
//
#ifndef NWOCAM
#define NWOCAM
#include <flgl/tools.h>
#include <flgl/glm.h>
#include "frame_manager.h"

class NWOcam : public OrthoCamera {
    frame_manager_t * frame_manager;
public:
    NWOcam(frame_manager_t * fmngr);

    virtual bool update_condition() const override;
    glm::mat4 const& updateProj() override;
    virtual void update() override;
};


#endif /* NWOCAM */
