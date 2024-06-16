#include "frame_manager.h"
#include "game/World.h"
#include <flgl/logger.h>
LOG_MODULE(fmngr);
using namespace glm;

bool frame_manager_t::update_wh(region_coords_t const& center, float camvw, float asp) {
    static region_coords_t pcenter{0xFFFFFFFF, 0xFFFFFFFF};
    int nx = ((int)ceil(camvw/(float)REGION_SIZE)+1)/2;
    int ny = ((int)ceil((camvw/(float)REGION_SIZE)/asp)+1)/2;

    bool needfbuff = false;
    if (nx != w || ny != h) {
        LOG_INF("\t%dx%d regions", (nx*2)+1, (ny*2)+1);
        needfbuff = true;
    }
    if (needfbuff || pcenter != center) {
        region_viewer.setup(center, nx, ny);
    }
    w = nx; h = ny;
    return needfbuff;
}

ivec2 frame_manager_t::get_fbuff_wh_pix() const {
    return ivec2(
        min((min((region_viewer.botright.x - region_viewer.topleft.x + 1), WORLD_DIAMETER) * REGION_SIZE) * 32, 8*16*32),
        min((min((region_viewer.topleft.y - region_viewer.botright.y + 1), WORLD_DIAMETER) * REGION_SIZE) * 32, 8*16*32)
    );
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
    // topleft  = center + glm::ivec2(-w, h);
    // botright = center + glm::ivec2(w, -h);
    // LOG_INF("expect %d,%d, getting %d,%d", min(2*w+1, WORLD_DIAMETER), min(2*h+1, WORLD_DIAMETER), botright.x - topleft.x + 1, topleft.y - botright.y + 1);
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
