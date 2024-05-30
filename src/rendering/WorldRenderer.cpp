#include "WorldRenderer.h"
#include <flgl/logger.h>
#include <flgl/glm.h>
LOG_MODULE(wrend);
using namespace glm;

void WorldRenderer::use_camera(OrthoCamera& c) {
    cam = &c;
}

void WorldRenderer::use_world(World& w) {
    world = &w;
}

void WorldRenderer::init() {
	RegionRenderer::static_init();
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
		renderers[i].use_region(&(world->regions[i]));
		renderers[i].init();
		renderers[i].prepare();
	}
}

void WorldRenderer::prepare() {

}

void WorldRenderer::render() {
	Renderer::clear();
	RegionRenderer::sync_camera(*cam);
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
		ivec2 const& rpos = world->regions[i].pos;
		if (abs((((float)REGION_SIZE*rpos.x)+(REGION_SIZE/2)) - (cam->readPos().x)) - REGION_SIZE/2 > (0.5*cam->readViewWidth())) continue;
		if (abs((((float)REGION_SIZE*rpos.y)+(REGION_SIZE/2)) - (cam->readPos().y)) - REGION_SIZE/2 > ((0.5*cam->readViewWidth()/window.aspect))) continue;
		renderers[i].prepare();
		renderers[i].render();
	}
}

void WorldRenderer::destroy() {
    RegionRenderer::static_destroy();
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
		renderers[i].destroy();
	}
}


