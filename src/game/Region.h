#ifndef REGION_H
#define REGION_H
#include <flgl/glm.h>
#include "ECS.h"

typedef unsigned short sprite_t;
typedef glm::ivec2 region_coords_t;
typedef glm::ivec2 tile_coords_t;

#define REGION_SIZE_LOG (4)
#define REGION_SIZE (1<<REGION_SIZE_LOG)

struct TerrainTile {
	uint16_t id;
	float friction;
	sprite_t img;
};

struct SurfaceTile {
	sprite_t img;
	entID ent;
	union {
		struct {
			bool solid 				: 1;
			bool blocks_light		: 1;
			bool blocks_selflight	: 1;
			bool __res				: 5;
		} f; // lol as if this is a hw register
		uint8_t val;
	} props;
};

struct Tile {
	TerrainTile terrain;
	SurfaceTile surface;
};

struct Region {
	Tile buffer[REGION_SIZE*REGION_SIZE];
	region_coords_t pos;

	Tile& tile_at(int x, int y);
	Tile const& read_tile_at(int x, int y) const;
	void clear_flag();
	bool read_flag() const;
	void raise_flag();

	Region() = default;
	Region(Region const& other);

	Region& operator=(Region const& other);

private:
	bool flag;
};


#endif
