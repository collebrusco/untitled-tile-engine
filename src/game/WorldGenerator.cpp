#include "WorldGenerator.h"

WorldGenerator::WorldGenerator(world_seed_t sd) : seed(sd) {}

void WorldGenerator::generate(int x, int y, Region* target) {
    for (int i = 0; i < REGION_SIZE; i++) {
        for (int j = 0; j < REGION_SIZE; j++) {
            target->tile_at(i,j) = {
                .img = (sprite_t)(i+j*REGION_SIZE)
            };
        }
    }
    target->pos = {x,y};
}

TestWorldGenerator::TestWorldGenerator(world_seed_t sd) : 
WorldGenerator(sd), gen(seed), dis(0) {
}

void TestWorldGenerator::generate(int x, int y, Region* target) {
    target->pos = {x,y};

    for (int i = 0; i < REGION_SIZE; i++) {
        for (int j = 0; j < REGION_SIZE; j++) {
            int r = dis(gen) & 0xF;
            sprite_t sp = 0;
            switch (r) {
            case 0:
                sp = 0; break;
            case 1:
            // case 2:
            // case 3:
                sp = 1; break;
            default:
                sp = 2; break;
            }
            target->tile_at(i,j) = {
                .img = sp
            };
        }
    }
}
