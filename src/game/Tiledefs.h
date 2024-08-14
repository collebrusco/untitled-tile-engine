/** 
 * Tiledefs.h
 * created 08/10/24 by frank collebrusco 
 */
#ifndef TILE_DEFS_H
#define TILE_DEFS_H

#include "data/World.h"
#include "data/State.h"

struct sTiledef {
    const SurfaceTile st;
    inline sTiledef(SurfaceTile t) : st(t) {}
    virtual void inline place(World& world, Tile* tile, tile_coords_t const pos) const {
        if (tile) {tile->surf = st; return;}
        world.tile_at(pos).surf = st;
    }
};

struct tTiledef {
    const TerrainTile tt;
    inline tTiledef(TerrainTile t) : tt(t) {}
    virtual void inline place(World& world, Tile* tile, tile_coords_t const pos) const {
        if (tile) {tile->terr = tt; return;}
        world.tile_at(pos).terr = tt;
    }
};


#endif /* TILE_DEFS_H */
