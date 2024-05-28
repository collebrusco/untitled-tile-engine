#include "WorldSave.h"
#include <flgl/logger.h>
using namespace glm;
LOG_MODULE(wsave);

WorldSave::WorldSave(bool iinf, size_t s) :
isinf(iinf), size(s), generator(0) 
{
}

WorldSave::~WorldSave() {}

bool WorldSave::bounds(int x, int y) const {
    return isinf || (
                        ((x + (int)(size/2)) >= 0)   &&
                        ((x + (int)(size/2)) < size) &&
                        ((y + (int)(size/2)) >= 0)   &&
                        ((y + (int)(size/2)) < size) 
                    );
}

void WorldSave::use_generator(WorldGenerator* gen) {generator = gen;}


MapWSave::MapWSave() : WorldSave(true) {}


void MapWSave::load(int x, int y, Region* target) {
    if (rmap.find({x,y}) == rmap.end()) {
        if (generator==0) {LOG_ERR("no generator for %d,%d", x, y); return;}
        generator->generate(x, y, target);
        store(*target); return;
    }
    *target = rmap.at({x,y});
}

bool MapWSave::store(Region const& target) {
    rmap.insert({target.pos, target}); return true;
}

