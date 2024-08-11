#include "Tiles.h"


sTiledef const sTiles::blank(
    (SurfaceTile) {
        .img = 0,
        .ent = ~(entID{0}),
        .props = {.val = 0}
    }
);

sTiledef const sTiles::wall(
    (SurfaceTile) {
        .img = 3,
        .ent = ~(entID{0}),
        .props = {
            .f = {
                .present = 1,
                .solid = 1,
                .blocks_light = 1,
                .__res = 0
            }
        }
    }
);



tTiledef const tTiles::sand(
    (TerrainTile) {
        .friction = 1.f
        .img = 2,
    }
);

tTiledef const tTiles::sand_pebble(
    (TerrainTile) {
        .friction = 0.8f
        .img = 0,
    }
);

tTiledef const tTiles::sand_tumble(
    (TerrainTile) {
        .friction = 0.5f
        .img = 1,
    }
);





void sTiles::destroy(World& world, SurfaceTile st) {
    /* if has onkill comp, call kill? */
    if (world.entityValid(st.ent)) world.removeEntity(st.ent);
}

void sTiles::destroy(World& world, tile_coords_t pos) {
    sTiles::destroy(world, world.tile_at(pos).surf);
}

void sTiles::destroy(World& world, glm::vec2 pos) {
    sTiles::destroy(world, World::pos_to_tpos(pos));
}

void sTiles::destroy_clear(World &world, tile_coords_t pos) {
    sTiles::destroy(world, pos);
    sTiles::blank.place(world, 0, pos);
}

void sTiles::destroy_clear(World &world, glm::vec2 pos) {
    sTiles::destroy_clear(world, World::pos_to_tpos(pos));
}
