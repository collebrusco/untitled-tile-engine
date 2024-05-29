#include "Region.h"


Tile& Region::tile_at(int x, int y) {
    flag = 1;
    return buffer[x+y*REGION_SIZE];
}

Tile const& Region::read_tile_at(int x, int y) {
    return buffer[x+y*REGION_SIZE];
}

void Region::clear_flag() {flag = 0;}
bool Region::read_flag() {return flag;}
