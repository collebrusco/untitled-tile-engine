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
private:
    virtual void user_create() override final;
    virtual void user_update(float dt, Keyboard const& kb, Mouse const& mouse) override final;
    virtual void user_render() override final;
    virtual void user_destroy() override final;

	World world;

	WorldRenderer wrenderer;
	TextRenderer text_renderer;
	
	OrthoCamera cam;

};

WorldDriver::WorldDriver() : GameDriver(),
							world(std::make_unique<MapWSave>(std::make_unique<TestWorldGenerator>(0xFACEFACE))),
							cam({0.f,0.f,1.f}, {0.f, 0.f, -1.f}, {0.f, 1.f, 0.f}, 0.001f, 10000.f, 64)
							{
							}

void WorldDriver::user_create() {
	Renderer::context_init("untitled", 1280, 720);
	cam.update();
	wrenderer.use_camera(cam);
	wrenderer.use_world(world);
	wrenderer.init();
	text_renderer.init_text_rendering();
	text_renderer.init();
	text_renderer.set_text("move with wasd | scroll to zoom | click to place walls\npress L to turn on lighting (bad) (wip) %s", "");
}

void WorldDriver::user_update(float dt, Keyboard const& kb, Mouse const& mouse) {
	if (kb[GLFW_KEY_ESCAPE].down) this->close();
	if (kb[GLFW_KEY_W].down) cam.getPos().y += dt * (8.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	if (kb[GLFW_KEY_A].down) cam.getPos().x -= dt * (8.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	if (kb[GLFW_KEY_S].down) cam.getPos().y -= dt * (8.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	if (kb[GLFW_KEY_D].down) cam.getPos().x += dt * (8.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	if (kb[GLFW_KEY_K].pressed) {static bool wf = 1;wrenderer.twf();wf = !wf;}
#ifdef __APPLE__
	if (abs(mouse.scroll.y) > 0.1) cam.getViewWidth() += dt * mouse.scroll.y * 10.f;
#else
	if (abs(mouse.scroll.y) > 0.1) cam.getViewWidth() += dt * mouse.scroll.y * 100.f;
#endif
	cam.update();

	if (mouse.left.down) {
		vec2 ssm = world.world_mpos(mouse.pos, window.frame, &cam);
		Tile& tile = world.tile_at(vec2(ssm.x, ssm.y));
		tile.surf = {
			.img = 3,
			// .props = {
			// 	.f.present = 1,
			// 	.f.blocks_light = 1,
			// 	.f.solid = 1
			// } // microsoft's compiler can't handle this (lame)
		};
		tile.surf.props.f.present = 
		tile.surf.props.f.blocks_light =
		tile.surf.props.f.solid = 1;
	}

	world.relocate(cam.readPos().xy());

	static size_t fpt = 0;
	fpt++;
	if (!(fpt % 60)) LOG_INF("fps: %.1f", 1./dt);
}

void WorldDriver::user_render() {
	wrenderer.give_mouse(window.mouse.pos);
	wrenderer.prepare();
	wrenderer.render();
	text_renderer.render(24, window.frame.y - 48, 4);
}

void WorldDriver::user_destroy() {
	Renderer::context_close();
}
















static WorldDriver driver;
int main() {
	driver.start();
	return 0;
}

