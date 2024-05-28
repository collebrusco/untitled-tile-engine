#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H
#include "Region.h"

typedef size_t world_seed_t;

struct WorldGenerator {
    const world_seed_t seed;
    WorldGenerator(world_seed_t sd);
    virtual void generate(int x, int y, Region* target);
};

#include <random>
struct TestWorldGenerator : public WorldGenerator {
    std::mt19937 gen; std::uniform_int_distribution<> dis;
    TestWorldGenerator(world_seed_t sd);
    virtual void generate(int x, int y, Region* target);
};

#endif
