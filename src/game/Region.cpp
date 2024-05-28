#include "Region.h"


Tile& Region::tile_at(int x, int y) {
    return buffer[x+y*REGION_SIZE];
}
