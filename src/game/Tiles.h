/** 
 * Tiles.h
 * created 08/10/24 by frank collebrusco 
 */
#ifndef TILES_H
#define TILES_H
#include "game/Tiledefs.h"
#include "World.h"

struct sTiles {

    static void destroy(World& world, SurfaceTile st);
    static void destroy(World& world, tile_coords_t pos);
    static void destroy(World& world, glm::vec2 pos);
    static void destroy_clear(World& world, tile_coords_t pos);
    static void destroy_clear(World& world, glm::vec2 pos);

    static const sTiledef blank;
    static const sTiledef wall;

};

struct tTiles {

    static const tTiledef sand;
    static const tTiledef sand_pebble;
    static const tTiledef sand_tumble;

};

#endif /* TILES_H */
