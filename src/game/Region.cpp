#include "Region.h"
#include <flgl/logger.h>
LOG_MODULE(reg);


Tile& Region::tile_at(int x, int y) {
    flag = 1;
    return buffer[x+y*REGION_SIZE];
}

Tile const& Region::read_tile_at(int x, int y) const {
    return buffer[x+y*REGION_SIZE];
}

void Region::clear_flag() {flag = 0;}
bool Region::read_flag() const {return flag;}
void Region::raise_flag() {flag = 1;}

Region::Region(Region const& other) { LOG_DBG("cpy");
    this->pos = other.pos;
    this->flag = 1;
    memcpy(buffer, other.buffer, sizeof(Tile) * REGION_SIZE * REGION_SIZE);
}

Region& Region::operator=(Region const& other) {
    if (this!=&other) { LOG_DBG("=");
        this->pos = other.pos;
        memcpy(buffer, other.buffer, sizeof(Tile) * REGION_SIZE * REGION_SIZE);
    }
    this->flag = 1;
    return *this;
}
