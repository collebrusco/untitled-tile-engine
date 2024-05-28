#include "World.h"

World::World(WorldSave& sv) : gen(0), save(&sv) {
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

Tile& World::tile_at(world_coords_t pos) {
    Region& r = region_at(pos/REGION_SIZE);
    pos += ((WORLD_DIAMETER/2) * REGION_SIZE);
    return r.tile_at(pos.x%REGION_SIZE, pos.y%REGION_SIZE);
}

Tile& World::tile_at(glm::vec2 pos) {
    world_coords_t p = (world_coords_t)pos;
    return tile_at(p);
}


