#include "WorldGenerator.h"

WorldGenerator::WorldGenerator(world_seed_t sd) : seed(sd) {}

void WorldGenerator::generate(int x, int y, Region* target) const {
    for (int i = 0; i < REGION_SIZE; i++) {
        for (int j = 0; j < REGION_SIZE; j++) {
            target->tile_at(i,j) = {
                .img = (sprite_t)(i+j*REGION_SIZE)
            };
        }
    }
    target->pos = {x,y};
}
