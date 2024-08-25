/** 
 * Tiles.h
 * created 08/10/24 by frank collebrusco 
 */
#ifndef TILES_H
#define TILES_H
#include "game/Tiledefs.h"
#include "data/World.h"
#include "game/oil.h"

struct oil_sTiledef final : public sTiledef {
    inline oil_sTiledef(SurfaceTile t) : sTiledef(t) {}
    virtual void inline place(World& world, Tile* tile, tile_coords_t const pos) const override final {
        entID* ent;
        if (tile) {
            ent = &tile->surf.ent; 
            tile->surf = st;
        } else {
            ent = &world.tile_at(pos).surf.ent;
            world.tile_at(pos).surf = st;
        }
        c_oilrig::place(world, tile->surf);
    }
};

struct sTiles {

    static void destroy(World& world, SurfaceTile st);
    static void destroy(World& world, tile_coords_t pos);
    static void destroy(World& world, glm::vec2 pos);
    static void destroy_clear(World& world, tile_coords_t pos);
    static void destroy_clear(World& world, glm::vec2 pos);

    static const sTiledef blank;
    static const sTiledef wall;
    static const oil_sTiledef oil_rig;

};

struct tTiles {

    static const tTiledef sand;
    static const tTiledef sand_pebble;
    static const tTiledef sand_tumble;
    static const tTiledef oil;

};

#endif /* TILES_H */
