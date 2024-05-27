#ifndef WORLD_H
#define WORLD_H
#include <flgl/glm.h>

typedef unsigned int sprite_t;

#define REGION_SIZE_LOG (3)
#define REGION_SIZE (1<<REGION_SIZE_LOG)

struct Tile {
	bool solid;
	float friction;
	sprite_t img;
};

struct Region {
	Tile buffer[REGION_SIZE*REGION_SIZE];
	glm::ivec2 reg_pos; // world pos * REGION_SIZE

	Tile& tile_at(int x, int y) {return buffer[x+y*REGION_SIZE];}

};


#endif
