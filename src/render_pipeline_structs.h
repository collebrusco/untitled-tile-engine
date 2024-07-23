// 
// render_pipeline_structs.h
// created 06/16/24 by frank collebrusco 
//
#ifndef RENDER_PIPELINE_STRUCTS
#define RENDER_PIPELINE_STRUCTS
#include <flgl/glm.h>
#include <flgl.h>
#include "game/World.h"

/**
 * ================ BUFF REND IN ================
 * ENGINE
 *   |____BUFFER_RENDERER
 *              |____WINDOW_RENDERER
 * 
 *  camera object pos in game world & window size (game coords)
 *      engine passes this to buffer renderer used to update frame size & pos
 *          engine renders world into framebuffer
 *              this struct is passed along w fbuffer to window renderer
 */

struct local_cam_t {
    glm::vec2 pos;
    glm::vec2 frame;
};

static inline float local_cam_asp(local_cam_t& cam) {
    return cam.frame.x / cam.frame.y;
}

static inline void local_cam_setvw(local_cam_t& lcam, float vw) {
    lcam.frame.x = vw;
}

static inline void local_cam_upf(local_cam_t& lcam) {
    lcam.frame.y = lcam.frame.x / window.aspect;
}

struct buffrender_details_t {
    World* world;
    glm::vec2 wmpos; /* mouse pos in world */
    local_cam_t lcam;
};

/**
 * ================ BUFFREND OUT / WINDREND IN ================
 *  BUFFER_RENDERER
 *       |_____WINDOW_RENDERER
 * 
 *  buffer renderer builds & paints framebuffer
 *      from that, 
 *      its frame in pixels, 
 *      the world pos of its bot left corner,
 *      and the local cam details,
 *          the window renderer can render the correct subregion
 */
struct postrender_details_t {
    Framebuffer     fbuf;
    Texture         fbtex;

    glm::ivec2 frame_pix;
    glm::ivec2 world_blpos; /* bot left pos tile coords */
    local_cam_t local_cam;
};

#endif /* RENDER_PIPELINE_STRUCTS */
