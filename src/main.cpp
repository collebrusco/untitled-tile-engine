#include <flgl.h>
#include <flgl/tools.h>
#include <flgl/glm.h>
#include <flgl/logger.h>
#include "Driver.h"
#include "rendering/RegionRenderer.h"
#include "game/World.h"
LOG_MODULE(main);

#include <iostream>
#include <vector>
using namespace glm;
using namespace std;




class Scopetimer {
    Stopwatch sw; string name;
public:
    Scopetimer(string n) : sw(MICROSECONDS), name(n) {sw.reset_start();}
    ~Scopetimer() {float time = sw.stop(); LOG_DBG("%s time %.0fus", name.c_str(), time);}
};




class WorldDriver : public GameDriver {
public:
	WorldDriver();
	virtual ~WorldDriver() = default;
private:
    virtual void user_create() override final;
    virtual void user_update(float dt, Keyboard const& kb, Mouse const& mouse) override final;
    virtual void user_render() override final;
    virtual void user_destroy() override final;

	// Region testr;
	// RegionRenderer renderer;

	MapWSave wsave;
	World world;

	RegionRenderer renderers[WORLD_DIAMETER*WORLD_DIAMETER];
	
	OrthoCamera cam;

};

WorldDriver::WorldDriver() : GameDriver(), 	
							// testr(), 
							// renderer(testr), 
							wsave(),
							world(wsave),
							cam({0.f,0.f,1.f}, {0.f, 0.f, -1.f}, {0.f, 1.f, 0.f}, 0.001f, 10000.f, 64)
							{
							}

void WorldDriver::user_create() {
	// testr = {
	// 	.pos = {0,0}
	// };
	// for (size_t i = 0; i < (REGION_SIZE*REGION_SIZE); i++)
	// 	testr.buffer[i].img = i%1024;
	Renderer::context_init("untitled", 1280, 720);
	cam.update();
	RegionRenderer::static_init();
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
		renderers[i].use_region(&(world.regions[i]));
		renderers[i].init();
		renderers[i].prepare();
	}
}

void WorldDriver::user_update(float dt, Keyboard const& kb, Mouse const& mouse) {
	if (kb[GLFW_KEY_ESCAPE].down) this->close();
	if (kb[GLFW_KEY_W].down) cam.getPos().y += dt * 2.f * ((kb[GLFW_KEY_LEFT_SHIFT].down+1.) * 3.f);
	if (kb[GLFW_KEY_A].down) cam.getPos().x -= dt * 2.f * ((kb[GLFW_KEY_LEFT_SHIFT].down+1.) * 3.f);
	if (kb[GLFW_KEY_S].down) cam.getPos().y -= dt * 2.f * ((kb[GLFW_KEY_LEFT_SHIFT].down+1.) * 3.f);
	if (kb[GLFW_KEY_D].down) cam.getPos().x += dt * 2.f * ((kb[GLFW_KEY_LEFT_SHIFT].down+1.) * 3.f);
	if (kb[GLFW_KEY_K].pressed) {static bool wf = 1;gl.wireframe(wf);wf = !wf;}
	if (abs(mouse.scroll.y) > 0.1) cam.getViewWidth() += dt * mouse.scroll.y * 10.f;
	cam.update();

	static uint tg = 0; tg++;
	if (!(tg%64)) {
		LOG_DBG("fps: %.1f", 1.f/dt);
	}

}

void WorldDriver::user_render() { Scopetimer rtimer("render");
	Renderer::clear();
	RegionRenderer::sync_shader(cam);
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
		ivec2 const& rpos = world.regions[i].pos;
		if (abs((((float)REGION_SIZE*rpos.x)+(REGION_SIZE/2)) - (cam.readPos().x)) - REGION_SIZE/2 > (cam.readViewWidth())) continue;
		if (abs((((float)REGION_SIZE*rpos.y)+(REGION_SIZE/2)) - (cam.readPos().y)) - REGION_SIZE/2 > ((cam.readViewWidth()/window.aspect))) continue;
		renderers[i].render();
	}

}

void WorldDriver::user_destroy() {
	Renderer::context_close();
}
















static WorldDriver driver;
int main() {
	driver.start();
	return 0;
}

