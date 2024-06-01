#include "World.h"
#include <flgl/logger.h>
using namespace glm;
LOG_MODULE(wrld);

#define WBUFSIZE (WORLD_DIAMETER*WORLD_DIAMETER)

#define index_mod(vec, m) \
(ivec2((vec.x%m)<0?(vec.x%m)+m:vec.x%m, (vec.y%m)<0?(vec.y%m)+m:vec.y%m))

World::World(std::unique_ptr<WorldSave> sv) : 
save(std::move(sv)), center{0,0} {
    for (int i = 0; i < WORLD_DIAMETER; i++) {
        for (int j = 0; j < WORLD_DIAMETER; j++) {
            ivec2 p(i-(WORLD_DIAMETER/2), j-(WORLD_DIAMETER/2));
            Region* tar = &regions[rpos_to_idx(p)];
            save->load(p.x, p.y, tar, this);
        }
    }
}

bool World::bounds(region_coords_t pos) const {
    ivec2 l(center.x-(WORLD_DIAMETER/2),   center.y-(WORLD_DIAMETER/2)  );
    ivec2 h(center.x+(WORLD_DIAMETER/2)-1, center.y+(WORLD_DIAMETER/2)-1);
    return !(pos.x < l.x || pos.y < l.y || pos.x > h.x || pos.y > h.y);
}

Region& World::region_at(region_coords_t pos) {
    if (bounds(pos)) {return regions[rpos_to_idx(pos)];}
    Region* reg = save->read(pos.x, pos.y);
    if (reg) return *reg;
    Region temp;
    save->load(pos.x, pos.y, &temp, this);
    save->store(temp, *this); // TODO wasteful: extra memcpy (lazy)
    return *(save->read(pos.x, pos.y));
}

Region const& World::read_region_at(region_coords_t pos) const {
    if (bounds(pos)) return regions[rpos_to_idx(pos)];
    Region* reg = save->read(pos.x, pos.y);
    if (reg) return *reg;
    LOG_ERR("invalid read reg (must be in wbuff) %d,%d", pos.x, pos.y);
    return regions[0];
}

Tile& World::tile_at(tile_coords_t pos) {
    Region& r = region_at(pos_to_rpos(pos));
    pos = index_mod(pos, REGION_SIZE);
    return r.tile_at(pos.x, pos.y);
}

Tile& World::tile_at(glm::vec2 pos) {
    return tile_at(pos_to_tpos(pos));
}

Tile const& World::read_tile_at(tile_coords_t pos) const {
    Region const& r = read_region_at(pos/REGION_SIZE);
    pos = index_mod(pos, REGION_SIZE);
    return r.read_tile_at(pos.x, pos.y);
}

Tile const& World::read_tile_at(glm::vec2 pos) const {
    return read_tile_at(pos_to_tpos(pos));
}

region_coords_t World::pos_to_rpos(vec2 pos) {
    tile_coords_t p = (tile_coords_t)pos;
    vec2 frpos = ((vec2)pos) / (float)(REGION_SIZE);
    frpos = floor(frpos);
    return (region_coords_t)frpos;
}

tile_coords_t World::pos_to_tpos(vec2 pos) {
    tile_coords_t p = (tile_coords_t)pos;
    vec2 frpos = ((vec2)pos);
    frpos = floor(frpos);
    return (tile_coords_t)(frpos);
}

region_coords_t World::tpos_to_rpos(tile_coords_t tpos) {
    return pos_to_rpos((vec2)(tpos));
}

tile_coords_t World::rpos_to_tpos(region_coords_t rpos) {
    return rpos * 16;
}

void World::relocate(region_coords_t newpos) {
    if (newpos == center) return;
    if (abs(newpos.x - center.x) > 1 ||
        abs(newpos.y - center.y) > 1) return; // TODO fix this garbo
    shift(newpos.x - center.x, newpos.y - center.y);
}

void World::relocate(vec2 newpos) {
    relocate(pos_to_rpos(newpos));
}

void World::shift(int dx, int dy) {
    if (abs(dx)>1 || abs(dy)>1) {LOG_ERR("invalid shift %d,%d",dx,dy);return;}
    center.x += dx;
    if (dx) {
        int starty = center.y - (WORLD_DIAMETER/2);
        for (int i = 0; i < WORLD_DIAMETER; i++) {
            int y = starty + i;
            int x = center.x + ((dx*(WORLD_DIAMETER/2)) - (dx>0));
            size_t idx = rpos_to_idx(region_coords_t(x,y));
            save->store(regions[idx], *this);
            save->load(x, y, &regions[idx], this);
        }
    }
    center.y += dy;
    if (dy) {
        int startx  = center.x - (WORLD_DIAMETER/2);
        int endx    = center.x + (WORLD_DIAMETER/2);
        for (int i = 0; i < WORLD_DIAMETER; i++) {
            int x = startx + i;
            int y = center.y + ((dy*(WORLD_DIAMETER/2)) - (dy>0));
            size_t idx = rpos_to_idx(region_coords_t(x,y));
            save->store(regions[idx], *this);
            save->load(x, y, &regions[idx], this);
        }
    }
}

void World::full_move(region_coords_t newpos) {
    // for (int i = 0; i < WORLD_DIAMETER; i++) {
    //     for (int j = 0; j < WORLD_DIAMETER; j++) {
    //         Region* tar = &regions[i + j*WORLD_DIAMETER];
    //         region_coords_t rpos = tar->pos;
    //     }
    // }
}

size_t World::rpos_to_idx(region_coords_t rpos) {
    ivec2 idxs = index_mod(rpos, WORLD_DIAMETER);
    size_t res = idxs.x + idxs.y * WORLD_DIAMETER;
    // LOG_DBG("\timd %d,%d to %d,%d",rpos.x,rpos.y,idxs.x,idxs.y);
    return res;
}
