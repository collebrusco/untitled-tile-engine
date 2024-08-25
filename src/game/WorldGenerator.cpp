#include "WorldGenerator.h"
#include "game/Tiles.h"
#include <flgl/logger.h>
LOG_MODULE(wgen);

WorldGenerator::WorldGenerator(world_seed_t sd) : seed(sd) {}

void WorldGenerator::generate(int x, int y, Region* target, World* const world) {
    (void)world;
    for (int i = 0; i < REGION_SIZE; i++) {
        for (int j = 0; j < REGION_SIZE; j++) {
            target->tile_at(i,j) = {
                .terr = {.friction = 1.f, .img = (tile_sprite_t)(i+j*REGION_SIZE)},
                .surf = {.img = 0, .ent = 0, .props = {.val = 0}}
            };
        }
    }
    target->pos = {x,y};
}

TestWorldGenerator::TestWorldGenerator(world_seed_t sd) : 
WorldGenerator(sd), gen(seed), dis(0) {
}

void TestWorldGenerator::generate(int x, int y, Region* target, World* const world) {
    (void)world;
    target->pos = {x,y};
    
    for (int i = 0; i < REGION_SIZE; i++) {
        for (int j = 0; j < REGION_SIZE; j++) {
            int r = dis(gen) & 0xFF;
            int r2 = dis(gen) & 0xFF;
            Tile* t = &(target->tile_at(i,j));
            switch (r) {
            case 0:
                tTiles::sand_pebble.place(*world, t, {i,j});
                break;
            case 1:
            case 2:
            case 3:
            case 4:
            case 5:
                tTiles::sand_tumble.place(*world, t, {i,j});
                break;

            case 255:
                if (r2 < 25) {
                    tTiles::oil.place(*world, t, {i,j});
                    break;
                }
            default:
                tTiles::sand.place(*world, t, {i,j});
                break;
            }
            sTiles::blank.place(*world, t, {i,j});
        }
    }
}
