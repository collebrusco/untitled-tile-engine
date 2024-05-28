#ifndef REGION_H
#define REGION_H
#include <flgl/glm.h>

typedef unsigned int sprite_t;
typedef glm::ivec2 region_coords_t;
typedef glm::ivec2 world_coords_t;

#define REGION_SIZE_LOG (4)
#define REGION_SIZE (1<<REGION_SIZE_LOG)

struct Tile {
	bool solid;
	float friction;
	sprite_t img;
};

struct Region {
	Tile buffer[REGION_SIZE*REGION_SIZE];
	region_coords_t pos;

	Tile& tile_at(int x, int y);

};


#endif
