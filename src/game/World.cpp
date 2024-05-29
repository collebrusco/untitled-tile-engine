#include "World.h"
#include <flgl/logger.h>
using namespace glm;
LOG_MODULE(wrld);

World::World(WorldSave& sv) : 
gen(0), save(&sv), center{0,0} {
    save->use_generator(&gen);
    for (int i = 0; i < WORLD_DIAMETER; i++) {
        for (int j = 0; j < WORLD_DIAMETER; j++) {
            Region* tar = &regions[i+j*WORLD_DIAMETER];
            save->load(i-(WORLD_DIAMETER/2), j-(WORLD_DIAMETER/2), tar);
        }
    }
}

bool World::bounds(region_coords_t pos) const {
    pos += (WORLD_DIAMETER/2);
    return  (pos.x >= 0 && pos.x <= (WORLD_DIAMETER-1)) &&
            (pos.y >= 0 && pos.y <= (WORLD_DIAMETER-1));
}

Region& World::region_at(region_coords_t pos) {
    pos += (WORLD_DIAMETER/2);
    return regions[pos.x + pos.y * WORLD_DIAMETER];
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

#define index_mod(vec, m) \
(ivec2(vec.x%m<0?(vec.x%m)+m:vec.x%m, vec.y%m<0?(vec.y%m)+m:vec.y%m))

void World::shift(int x, int y) {
    if (abs(x)>1 || abs(y)>1) {LOG_ERR("invalid shift %d,%d",x,y);return;}
    center += ivec2(x,y);
}

size_t World::rpos_to_idx(region_coords_t rpos) const {
    rpos = rpos - center;
    rpos += (REGION_SIZE/2);
    if (rpos.x < 0 || rpos.y < 0 || 
        rpos.x > REGION_SIZE-1 || rpos.y > REGION_SIZE-1) 
            {LOG_ERR("invalid r2i %d,%d",rpos.x,rpos.y);return-1;}
    ivec2 idxs;
    return 0; // REPLACE
}
