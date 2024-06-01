#ifndef WORLD_SAVE_H
#define WORLD_SAVE_H
#include <memory>
#include "Region.h"
#include "WorldGenerator.h"

struct World;

/*
    as far as the world save is concerned,
    coords are region scale (1 unit = 1 region)
    and centered at 0,0
    for finite saves this ranges (-size/2 <= x,y < size/2)
*/
struct WorldSave {
    WorldSave(std::unique_ptr<WorldGenerator> gen);
    virtual ~WorldSave();
    bool bounds(int x, int y) const;

    std::unique_ptr<WorldGenerator> generator;

    virtual void load(int x, int y, Region* target, World* world) = 0;
    virtual void store(Region const& target, World const& world) = 0;
    virtual Region* read(int x, int y) = 0;
    // in the future, save to file
};

namespace std {
    template <>
    struct hash<glm::ivec2> {
        size_t operator()(glm::ivec2 vec) const noexcept {
            size_t result = 0;
            size_t a = static_cast<size_t>(vec.x);
            size_t b = static_cast<size_t>(vec.y);
            result = (a<<32) | b;
            return result;
        }
    };
}

#include <unordered_map>
typedef std::unordered_map<glm::ivec2, Region> region_map_t;
struct MapWSave : public WorldSave {
    MapWSave(std::unique_ptr<WorldGenerator> gen);
    virtual void load(int x, int y, Region* target, World* world) override final;
    virtual void store(Region const& target, World const& world) override final;
    virtual Region* read(int x, int y) override final;
private:
    region_map_t rmap;
};

#endif
