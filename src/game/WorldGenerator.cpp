#include "WorldGenerator.h"

WorldGenerator::WorldGenerator(world_seed_t sd) : seed(sd) {}

void WorldGenerator::generate(int x, int y, Region* target, World* world) {
    for (int i = 0; i < REGION_SIZE; i++) {
        for (int j = 0; j < REGION_SIZE; j++) {
            target->tile_at(i,j) = {
                .terr = {.img = (sprite_t)(i+j*REGION_SIZE)},
                .surf = {.img = 0, .ent = 0, .props = {.val = 0}}
            };
        }
    }
    target->pos = {x,y};
}

TestWorldGenerator::TestWorldGenerator(world_seed_t sd) : 
WorldGenerator(sd), gen(seed), dis(0) {
}

void TestWorldGenerator::generate(int x, int y, Region* target, World* world) {
    target->pos = {x,y};
    
    for (int i = 0; i < REGION_SIZE; i++) {
        for (int j = 0; j < REGION_SIZE; j++) {
            int r = dis(gen) & 0x3F;
            sprite_t sp = 0;
            switch (r) {
            case 0:
                sp = 0; break;
            case 1:
            case 2:
            case 3:
                sp = 1; break;
            default:
                sp = 2; break;
            }

            target->tile_at(i,j) = {
                .terr = {.img = sp},
                .surf = {.img = 0, .ent = 0, .props = {.val = 0}}
            };
        }
    }
}
