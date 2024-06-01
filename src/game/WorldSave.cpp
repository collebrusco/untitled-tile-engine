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
        if (generator==0) {LOG_ERR("no generator for %d,%d", x, y); return;}
        // LOG_DBG("gen %d,%d", x, y);
        generator->generate(x, y, target);
        return;
    }
    *target = rmap.at({x,y});
}

void MapWSave::store(Region const& target, World const& world) {
    // for (int i = 0; i < REGION_SIZE*REGION_SIZE; i++)
    //     if (target.buffer[i].img == 99) LOG_DBG("storing 99 %d,%d", target.pos.x, target.pos.y);
    rmap.emplace(target.pos, target);
}

Region* MapWSave::read(int x, int y) {
    if (rmap.find({x,y}) == rmap.end()) return 0;
    return &(rmap[{x,y}]);
}

