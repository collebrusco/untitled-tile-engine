#ifndef REGION_H
#define REGION_H
#include <flgl/glm.h>

typedef unsigned int sprite_t;
typedef glm::ivec2 region_coords_t;
typedef glm::ivec2 tile_coords_t;

#define REGION_SIZE_LOG (4)
#define REGION_SIZE (1<<REGION_SIZE_LOG)

struct Tile {
	uint16_t id;
	bool solid;
	float friction;
	sprite_t img;
	bool operator==(Tile const& a) const {return a.img==this->img;}
	bool operator==(Tile& a) {return a.img==this->img;}
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
