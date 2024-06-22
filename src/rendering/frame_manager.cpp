#include "frame_manager.h"
#include "game/World.h"
#include <flgl/logger.h>
#include "constants.h"
LOG_MODULE(fmngr);
using namespace glm;

bool frame_manager_t::update_wh(region_coords_t const& center, float camvw, float asp) {
    static region_coords_t pcenter{0xFFFFFFFF, 0xFFFFFFFF};
    int nx = ((int)ceil(camvw/(float)REGION_SIZE)+1)/2;
    int ny = ((int)ceil((camvw/(float)REGION_SIZE)/asp)+1)/2;
    // LOG_INF("cvw %.1f", camvw);

    bool needfbuff = false;
    if (nx != w || ny != h) {
        // LOG_INF("\t%dx%d regions", (nx*2)+1, (ny*2)+1);
        needfbuff = true;
    // LOG_INF("from %d,%d to %d,%d", w, h, nx, ny);
    }
    if (needfbuff || pcenter != center) {
        region_viewer.setup(center, nx, ny);
        pcenter = center;
    }
    w = nx; h = ny;
    return needfbuff;
}

/* TODO maybe pass this in in a smarter way or not */
#include <flgl.h>
/**
 * get framebuffer current actual size in pixels
 * do not use this for in game world size
 */
ivec2 frame_manager_t::get_fbuff_wh_pix() const {
    ivec2 res = ivec2(
        min((region_viewer.botright.x - region_viewer.topleft.x + 1), WORLD_DIAMETER) * REGION_SIZE * TILE_PIXELS,
        min((region_viewer.topleft.y - region_viewer.botright.y + 1), WORLD_DIAMETER) * REGION_SIZE * TILE_PIXELS
    );
    uint8_t ct = 0;
    while (res.x > min(FBUF_RES_LIMIT, window.frame.x*2) || res.y > min(FBUF_RES_LIMIT, window.frame.y*2)) {
        res /= 2; ct++;
    }
    return res;
}

glm::ivec2 frame_manager_t::get_fbuff_wh_tile() const {
    return ivec2(
        min((region_viewer.botright.x - region_viewer.topleft.x + 1), WORLD_DIAMETER) * REGION_SIZE,
        min((region_viewer.topleft.y - region_viewer.botright.y + 1), WORLD_DIAMETER) * REGION_SIZE
    );
}

glm::ivec2 frame_manager_t::get_fbuff_wh_region() const {
    return ivec2(
        min((region_viewer.botright.x - region_viewer.topleft.x + 1), WORLD_DIAMETER),
        min((region_viewer.topleft.y - region_viewer.botright.y + 1), WORLD_DIAMETER)
    );
}

float frame_manager_t::get_fbuff_aspect() const {
    return (float)(region_viewer.botright.x - region_viewer.topleft.x + 1) 
    / (float)(region_viewer.topleft.y - region_viewer.botright.y + 1);
}
    
void frame_manager_t::frame_region_view_t::setup(region_coords_t const& center, int w, int h) {
    topleft  = center + ivec2(-(min(WORLD_DIAMETER/2, w)), min((WORLD_DIAMETER/2)-1, h));
    botright = center + ivec2(min((WORLD_DIAMETER/2)-1, w), -(min(WORLD_DIAMETER/2, h)));
}

frame_manager_t::frame_region_view_t::frame_region_iter frame_manager_t::frame_region_view_t::begin() {
    frame_region_iter it(topleft);
    it.tl = &topleft; it.br = &botright;
    return it;
}

frame_manager_t::frame_region_view_t::frame_region_iter frame_manager_t::frame_region_view_t::end() {
    return frame_region_iter(glm::ivec2(botright.x+1, botright.y), 0);
}

frame_manager_t::frame_region_view_t::frame_region_iter::
frame_region_iter(region_coords_t const& p, region_coords_t *b) 
: pos(p), br(b) {}

bool frame_manager_t::frame_region_view_t::frame_region_iter::operator==(frame_region_iter const& other) const {
    return (!this->br && !other.br) || (this->pos == other.pos);
}
bool frame_manager_t::frame_region_view_t::frame_region_iter::operator!=(frame_region_iter const& other) const {
    return !this->operator==(other);
}

frame_manager_t::frame_region_view_t::frame_region_iter& frame_manager_t::frame_region_view_t::frame_region_iter::operator++() {
    if (!br) return *this;
    if (*br == pos) {
        br = 0;
        return *this;
    }
    pos.x++;
    if (pos.x > br->x) {
        pos.x = tl->x;
        pos.y--;
    }
    return *this;
}

region_coords_t frame_manager_t::frame_region_view_t::frame_region_iter::operator*() const {
    return pos;
}

frame_manager_t::frame_region_view_t frame_manager_t::regions_in_frame() const {
    return region_viewer;
}
