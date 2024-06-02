#include <flgl.h>
#include <flgl/tools.h>
#include <flgl/glm.h>
#include <flgl/logger.h>
#include "Driver.h"
#include "rendering/RegionRenderer.h"
#include "rendering/WorldRenderer.h"
#include "game/World.h"
LOG_MODULE(main);

#include <iostream>
#include <vector>
using namespace glm;
using namespace std;




class Scopetimer {
    Stopwatch sw; string name; size_t mod;
public:
    Scopetimer(string n, size_t m = 5) : sw(MICROSECONDS), name(n), mod(m) {sw.reset_start();}
    ~Scopetimer() {float time = sw.stop(); LOG_DBG("%s time %.0fus", name.c_str(), time);}
};




class WorldDriver : public GameDriver {
public:
	WorldDriver();
	virtual ~WorldDriver() = default;
	vec2 world_mpos(Mouse const& m);
private:
    virtual void user_create() override final;
    virtual void user_update(float dt, Keyboard const& kb, Mouse const& mouse) override final;
    virtual void user_render() override final;
    virtual void user_destroy() override final;

	World world;

	WorldRenderer wrenderer;
	
	OrthoCamera cam;

};

WorldDriver::WorldDriver() : GameDriver(),
							world(std::make_unique<MapWSave>(std::make_unique<TestWorldGenerator>(0xFACEFACE))),
							cam({0.f,0.f,1.f}, {0.f, 0.f, -1.f}, {0.f, 1.f, 0.f}, 0.001f, 10000.f, 64)
							{
							}

vec2 WorldDriver::world_mpos(Mouse const& m) {
	vec4 ssm = {m.pos.x,m.pos.y,0.,1.};
	ssm.x /= window.frame.x; ssm.y /= window.frame.y;
	ssm.x *= 2.f; ssm.x -= 1.f;
	ssm.y *= 2.f; ssm.y = 2.f - ssm.y; ssm.y -= 1.f;
	ssm = cam.iview() * (cam.iproj() * ssm);
	return ssm;
}

void WorldDriver::user_create() {
	Renderer::context_init("untitled", 1280, 720);
	cam.update();
	wrenderer.use_camera(cam);
	wrenderer.use_world(world);
	wrenderer.init();
}

void WorldDriver::user_update(float dt, Keyboard const& kb, Mouse const& mouse) {
	if (kb[GLFW_KEY_ESCAPE].down) this->close();
	if (kb[GLFW_KEY_W].down) cam.getPos().y += dt * (8.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	if (kb[GLFW_KEY_A].down) cam.getPos().x -= dt * (8.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	if (kb[GLFW_KEY_S].down) cam.getPos().y -= dt * (8.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	if (kb[GLFW_KEY_D].down) cam.getPos().x += dt * (8.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	if (kb[GLFW_KEY_K].pressed) {static bool wf = 1;gl.wireframe(wf);wf = !wf;}
	if (abs(mouse.scroll.y) > 0.1) cam.getViewWidth() += dt * mouse.scroll.y * 10.f;
	cam.update();

	if (mouse.left.down) {
		vec2 ssm = world_mpos(mouse);
		Tile& tile = world.tile_at(vec2(ssm.x, ssm.y));
		tile.surf = {
			.props = {
				.f.present = 1,
				.f.blocks_light = 1,
				.f.solid = 1
			}
		};
	}

	world.relocate(cam.readPos().xy());
}

void WorldDriver::user_render() {
	wrenderer.render();
}

void WorldDriver::user_destroy() {
	Renderer::context_close();
}
















static WorldDriver driver;
int main() {
	driver.start();
	return 0;
}

