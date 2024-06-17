#include <flgl.h>
#include <flgl/tools.h>
#include <flgl/glm.h>
#include <flgl/logger.h>
#include "Driver.h"
#include "rendering/RegionRenderer.h"
#include "rendering/WorldRenderer.h"
#include "game/World.h"
#include "render_pipeline_structs.h"
#include "rendering/BufferRenderer.h"
#include "rendering/PostRenderer.h"
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

	BufferRenderer brenderer;
	PostRenderer prenderer;
	TextRenderer text_renderer;

	local_cam_t lcam;

};

WorldDriver::WorldDriver() : GameDriver(),
							world(std::make_unique<MapWSave>(std::make_unique<TestWorldGenerator>(0xFACEFACE)))
							{
							}

void WorldDriver::user_create() {
	Renderer::context_init("untitled", 720, 720);
	lcam.frame = vec2(REGION_SIZE*2.f, (REGION_SIZE*2.f)/window.aspect);
	(*(brenderer.input_ptr())) = {
		.world = &world,
		.wmpos = vec2(0.f) /* TODO fuck */,
		.lcam = this->lcam
	};
	brenderer.init();
	prenderer.init();
	LOG_DBG("renderers init");
	text_renderer.init_text_rendering();
	text_renderer.init();
	text_renderer.set_text("move with wasd | scroll to zoom | click to place walls\npress L to turn on lighting (bad) (wip) %s", "");
	LOG_DBG("user driver created");
}

void WorldDriver::user_update(float dt, Keyboard const& kb, Mouse const& mouse) {
	if (kb[GLFW_KEY_ESCAPE].down) this->close();
	if (kb[GLFW_KEY_W].down) lcam.pos.y += dt * (.1f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	if (kb[GLFW_KEY_A].down) lcam.pos.x -= dt * (.1f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	if (kb[GLFW_KEY_S].down) lcam.pos.y -= dt * (.1f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	if (kb[GLFW_KEY_D].down) lcam.pos.x += dt * (.1f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	world.relocate(lcam.pos);

	if (abs(mouse.scroll.y) > 0.1) {
#ifdef __APPLE__
		float vwadd = dt * mouse.scroll.y * 100.f;
#else
		float vwadd = dt * mouse.scroll.y * 1000.f;
#endif
		local_cam_setvw(lcam, glm::clamp(lcam.frame.x + vwadd, 8.f, REGION_SIZE*512.f));
		// LOG_INF("changing vw to %.1f,%.1f", lcam.frame.x, lcam.frame.y);
	}


	// ugh TODO fix mouse
	if (mouse.left.down) {
		vec2 ssm = world.world_mpos(mouse.pos, window.frame, &brenderer.cam);
		Tile& tile = world.tile_at(vec2(ssm.x, ssm.y));
		tile.surf = {
			.img = 3,
			.props = {
				.f.present = 1,
				.f.blocks_light = 1,
				.f.solid = 1
			} // microsoft's compiler can't handle this (lame)
		};
		tile.surf.props.f.present = 
		tile.surf.props.f.blocks_light =
		tile.surf.props.f.solid = 1;
	}


	static size_t fpt = 0;
	fpt++;
	if (!(fpt % 1024)) LOG_INF("fps: %.1f", 1./dt);
}

void WorldDriver::user_render() {
	// input data from engine into buf renderer
	(*(brenderer.input_ptr())) = {
		.world = &world,
		.wmpos = vec2(0.f) /* TODO fuck */,
		.lcam = this->lcam
	};
	// buffer render to framebuffer
	brenderer.prepare();
	brenderer.render();
	// transfer buf render output to post render input
	brenderer.write_output(prenderer.input_ptr());
	// world frame to window
	prenderer.prepare();
	prenderer.render();

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

