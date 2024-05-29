#ifndef WORLD_H
#define WORLD_H
#include <flgl/glm.h>
#include "WorldGenerator.h"
#include "WorldSave.h"
#include "Region.h"


#define WORLD_DIAMETER (8)
struct World {
    World(WorldSave& sv);

    Region regions[WORLD_DIAMETER*WORLD_DIAMETER];

    bool bounds(region_coords_t pos) const;
    Region& region_at(region_coords_t pos);
    Tile& tile_at(tile_coords_t pos);
    Tile& tile_at(glm::vec2 pos);

    void shift(int x, int y);

private:

    WorldGenerator gen;
    WorldSave* save;

    region_coords_t center;

    size_t rpos_to_idx(region_coords_t rpos) const;
};


#endif
