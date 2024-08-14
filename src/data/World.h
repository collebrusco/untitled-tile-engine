#ifndef WORLD_H
#define WORLD_H
#include <memory>
#include <flgl/glm.h>
#include <flgl/tools.h>
#include "game/WorldGenerator.h"
#include "data/WorldSave.h"
#include "data/Region.h"
#include "ECS.h"
#include "constants.h"

struct World : public ECS {
    explicit World(std::unique_ptr<WorldSave> sv);

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
    static glm::vec2 tpos_to_pos(tile_coords_t tpos);
    static tile_coords_t rpos_to_tpos(region_coords_t rpos);

    void relocate(region_coords_t newpos);
    void relocate(glm::vec2 newpos);
    region_coords_t get_center() const;

    Region regions[WORLD_DIAMETER*WORLD_DIAMETER];

    static size_t rpos_to_idx(region_coords_t rpos); // renderer needs this
private:
    void shift(int dx, int dy);
    void full_move(region_coords_t newpos);

    std::unique_ptr<WorldSave> save;

    region_coords_t center;

};


#endif
