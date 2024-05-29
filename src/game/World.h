#ifndef WORLD_H
#define WORLD_H
#include <flgl/glm.h>
#include "WorldGenerator.h"
#include "WorldSave.h"
#include "Region.h"


#define WORLD_DIAMETER (4)
struct World {
    World(WorldSave& sv);

    Region regions[WORLD_DIAMETER*WORLD_DIAMETER];

    bool bounds(region_coords_t pos) const;
    Region& region_at(region_coords_t pos);
    Region const& read_region_at(region_coords_t pos) const;
    Tile& tile_at(tile_coords_t pos);
    Tile& tile_at(glm::vec2 pos);
    Tile const& read_tile_at(tile_coords_t pos) const;
    Tile const& read_tile_at(glm::vec2 pos) const;
    static region_coords_t pos_to_rpos(glm::vec2 pos);
    static tile_coords_t pos_to_tpos(glm::vec2 pos);
    static region_coords_t tpos_to_rpos(tile_coords_t tpos);
    static tile_coords_t rpos_to_tpos(region_coords_t rpos);

    void shift(int dx, int dy);

private:

    TestWorldGenerator gen;
    WorldSave* save;

    region_coords_t center;

    static size_t rpos_to_idx(region_coords_t rpos);
};


#endif
