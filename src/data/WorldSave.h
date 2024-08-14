#ifndef WORLD_SAVE_H
#define WORLD_SAVE_H
#include <memory>
#include "data/Region.h"
#include "game/WorldGenerator.h"

// The world save is responsible for storing world state that goes out of range
// of the fast circ buffer in the World struct. that buffer pulls from and
// stores to this saver. This keeps the whole world in ram for now
// but should eventually have the ability to write to disk (yikes)

struct World;

struct WorldSave {
    WorldSave(std::unique_ptr<WorldGenerator> gen);
    virtual ~WorldSave();
    bool bounds(int x, int y) const;

    std::unique_ptr<WorldGenerator> generator;

    virtual void load(int x, int y, Region* target, World* const world) = 0;
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
struct MapWSave final : public WorldSave {
    MapWSave(std::unique_ptr<WorldGenerator> gen);
    virtual void load(int x, int y, Region* target, World* const world) override final;
    virtual void store(Region const& target, World const& world) override final;
    virtual Region* read(int x, int y) override final;
private:
    region_map_t rmap;
};

#endif
