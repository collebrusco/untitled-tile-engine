#include "World.h"
#include <flgl/logger.h>
using namespace glm;
LOG_MODULE(wrld);

#define index_mod(vec, m) \
(ivec2((vec.x%m)<0?(vec.x%m)+m:vec.x%m, (vec.y%m)<0?(vec.y%m)+m:vec.y%m))

World::World(WorldSave& sv) : 
gen(0), save(&sv), center{0,0} {
    save->use_generator(&gen);
    for (int i = 0; i < WORLD_DIAMETER; i++) {
        for (int j = 0; j < WORLD_DIAMETER; j++) {
            ivec2 p(i-(WORLD_DIAMETER/2), j-(WORLD_DIAMETER/2));
            Region* tar = &regions[rpos_to_idx(p)];
            save->load(p.x, p.y, tar);
        }
    }
}

bool World::bounds(region_coords_t pos) const {
    pos += (WORLD_DIAMETER/2);
    return  (pos.x >= 0 && pos.x <= (WORLD_DIAMETER-1)) &&
            (pos.y >= 0 && pos.y <= (WORLD_DIAMETER-1));
}

Region& World::region_at(region_coords_t pos) {
    return regions[rpos_to_idx(pos)];
}

Tile& World::tile_at(tile_coords_t pos) {
    vec2 frpos = ((vec2)pos) / (float)(REGION_SIZE);
    frpos = floor(frpos);
    Region& r = region_at((ivec2)frpos);
    LOG_DBG("reg at %.1f,%.1f", frpos.x, frpos.y);
    pos += ((WORLD_DIAMETER/2) * REGION_SIZE);
    pos.x %= REGION_SIZE; pos.y %= REGION_SIZE;
    LOG_DBG("tileat %d,%d", pos.x, pos.y);
    return r.tile_at(pos.x, pos.y);
}

Tile& World::tile_at(glm::vec2 pos) {
    LOG_DBG("vec pos %.1f,%.1f", pos.x, pos.y);
    tile_coords_t p = (tile_coords_t)pos;
    p.x -= (pos.x<0); p.y -= (pos.y<0);
    LOG_DBG("int pos %d,%d", p.x, p.y);
    return tile_at(p);
}

region_coords_t World::pos_to_rpos(vec2 pos) {
    tile_coords_t p = (tile_coords_t)pos;
    vec2 frpos = ((vec2)pos) / (float)(REGION_SIZE);
    frpos = floor(frpos);
    return (region_coords_t)frpos;
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
            // LOG_DBG("repl %d,%d at %d with %d,%d", regions[idx].pos.x, regions[idx].pos.y, idx, x, y);
            save->store(regions[idx]); save->load(x, y, &regions[idx]);
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
            save->store(regions[idx]); save->load(x, y, &regions[idx]);
        }
    }
}

size_t World::rpos_to_idx(region_coords_t rpos) {
    ivec2 idxs = index_mod(rpos, WORLD_DIAMETER);
    size_t res = idxs.x + idxs.y * WORLD_DIAMETER;
    // LOG_DBG("\timd %d,%d to %d,%d",rpos.x,rpos.y,idxs.x,idxs.y);
    return res;
}
