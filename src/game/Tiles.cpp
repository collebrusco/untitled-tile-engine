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
        .props.f.present = 1,
        .props.f.blocks_light = 1,
        .props.f.solid = 1
    }
);



tTiledef const tTiles::sand(
    (TerrainTile) {
        .img = 2,
        .friction = 1.f
    }
);

tTiledef const tTiles::sand_pebble(
    (TerrainTile) {
        .img = 0,
        .friction = 0.8f
    }
);

tTiledef const tTiles::sand_tumble(
    (TerrainTile) {
        .img = 1,
        .friction = 0.5f
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
