#ifndef WORLD_H
#define WORLD_H
#include <flgl/glm.h>
#include "WorldGenerator.h"
#include "WorldSave.h"
#include "Region.h"


#define WORLD_DIAMETER (16)
struct World {
    World(WorldSave& sv);

    Region regions[WORLD_DIAMETER*WORLD_DIAMETER];

    bool bounds(region_coords_t pos) const;
    Region& region_at(region_coords_t pos);
    Tile& tile_at(world_coords_t pos);
    Tile& tile_at(glm::vec2 pos);

private:

    TestWorldGenerator gen;
    WorldSave* save;

};


#endif
