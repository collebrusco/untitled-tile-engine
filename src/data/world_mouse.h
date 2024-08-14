/** 
 * world_mouse.h
 * created 08/08/24 by frank collebrusco 
 */
#ifndef WORLD_MOUSE_H
#define WORLD_MOUSE_H
#include <type_traits>
#include <flgl.h>
#include <flgl/glm.h>
#include "util/misc.h"

struct world_mouse_t {
	const Mouse* mouse;
	glm::vec2 pos, delt;
};
ASSERT_AGGREGATE(world_mouse_t);

#endif /* WORLD_MOUSE_H */
