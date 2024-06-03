#include "Region.h"
#include <flgl/logger.h>
LOG_MODULE(reg);


Tile& Region::tile_at(int x, int y) {
    flags.shad = flags.terr = 1;
    return buffer[x+y*REGION_SIZE];
}

Tile const& Region::read_tile_at(int x, int y) const {
    return buffer[x+y*REGION_SIZE];
}

void Region::clear_flag() {flags.terr = 0;}
bool Region::read_flag() const {return flags.terr;}
void Region::raise_flag() {flags.terr = 1;}
void Region::clear_sflag() {flags.shad = 0;}
bool Region::read_sflag() const {return flags.shad;}
void Region::raise_sflag() {flags.shad = 1;}

Region::Region(Region const& other) {
    this->pos = other.pos;
    this->flags.shad = this->flags.terr = 1;
    memcpy(buffer, other.buffer, sizeof(Tile) * REGION_SIZE * REGION_SIZE);
}

Region& Region::operator=(Region const& other) {
    if (this!=&other) {
        this->pos = other.pos;
        memcpy(buffer, other.buffer, sizeof(Tile) * REGION_SIZE * REGION_SIZE);
    }
    this->flags.shad = this->flags.terr = 1;
    return *this;
}
