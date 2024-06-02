#include "WorldSave.h"
#include <flgl/logger.h>
using namespace glm;
LOG_MODULE(wsave);

WorldSave::WorldSave(std::unique_ptr<WorldGenerator> gen) :
generator(std::move(gen)) 
{
}

WorldSave::~WorldSave() {}

bool WorldSave::bounds(int x, int y) const {
    return true;
}

MapWSave::MapWSave(std::unique_ptr<WorldGenerator> gen) : WorldSave(std::move(gen)) {}


void MapWSave::load(int x, int y, Region* target, World* world) {
    if (rmap.find({x,y}) == rmap.end()) {
        generator->generate(x, y, target, world);
        return;
    }
    *target = rmap.at({x,y});
}

void MapWSave::store(Region const& target, World const& world) {
    rmap.emplace(target.pos, target);
}

Region* MapWSave::read(int x, int y) {
    if (rmap.find({x,y}) == rmap.end()) return 0;
    return &(rmap[{x,y}]);
}

