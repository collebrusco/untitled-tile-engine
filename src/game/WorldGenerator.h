#ifndef WORLD_GENERATOR_H
#define WORLD_GENERATOR_H
#include "Region.h"

struct World;

typedef size_t world_seed_t;

struct WorldGenerator {
    const world_seed_t seed;
    WorldGenerator(world_seed_t sd);
    virtual ~WorldGenerator() = default;
    virtual void generate(int x, int y, Region* target, World* world);
};

#include <random>
struct TestWorldGenerator final : public WorldGenerator {
    std::mt19937 gen; std::uniform_int_distribution<> dis;
    TestWorldGenerator(world_seed_t sd);
    virtual void generate(int x, int y, Region* target, World* world);
};

#endif
