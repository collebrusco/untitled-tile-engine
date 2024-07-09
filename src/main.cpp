#include <flgl.h>
#include <flgl/tools.h>
#include <flgl/glm.h>
#include <flgl/logger.h>
#include "Driver.h"
#include "rendering/RegionRenderer.h"
#include "game/World.h"
#include "render_pipeline_structs.h"
#include "rendering/BufferRenderer.h"
#include "rendering/PostRenderer.h"
#include "game/State.h"
#include "game/Engine.h"
#include "game/system_player.h"
#include "game/components.h"
LOG_MODULE(main);

#include <iostream>
#include <vector>
using namespace glm;
using namespace std;



class WorldDriver : public GameDriver {
public:
	WorldDriver();
	virtual ~WorldDriver() = default;
private:
    virtual void user_create() override final;
    virtual void user_update(float dt, Keyboard const& kb, Mouse const& mouse) override final;
    virtual void user_render() override final;
    virtual void user_destroy() override final;

	vec2 world_mpos() const;
	vec2 world_mdelt() const;

	State state;

	/* Rendering stuff to be moved to one container */
	BufferRenderer brenderer;
	PostRenderer prenderer;
	TextRenderer text_renderer;

	Engine engine;

};

WorldDriver::WorldDriver() : GameDriver()
							{
							}

void WorldDriver::user_create() {
	/* ENGINE */
	engine.syslist.next_system() = PLAYER_SYSTEM_STRUCT;
	engine.init(&state);

	/* RENDERING */
	gl.init(); window.create("untitled", 980, 720);
	state.lcam.frame = vec2(REGION_SIZE*2.f, (REGION_SIZE*2.f)/window.aspect);
	(*(brenderer.input_ptr())) = {
		.world = &state.world,
		.wmpos = world_mpos(),
		.lcam = this->state.lcam
	};
	brenderer.init();
	prenderer.init();
	LOG_DBG("renderers init");
	text_renderer.init_text_rendering();
	text_renderer.init();
	text_renderer.set_text("move with wasd | scroll to zoom | click to place walls%s","");//\npress L to turn on lighting (bad) (wip) %s", "");
	LOG_DBG("user driver created");
}

vec2 WorldDriver::world_mpos() const {
	vec2 res = vec2(window.mouse.pos / (vec2)window.frame);
	res.y = 1.f - res.y;
	res = res * state.lcam.frame; /* tile coords relative to origin */
	res += state.lcam.pos - (state.lcam.frame/2.f);
	return res;
}

vec2 WorldDriver::world_mdelt() const {
	vec2 del = vec2(window.mouse.delta / (vec2)window.frame);
	del.y = 1.f - del.y;
	del = del * state.lcam.frame; /* tile coords */
	return del;
}

void WorldDriver::user_update(float dt, Keyboard const& kb, Mouse const& mouse) {

	engine.step(dt, &state, kb, mouse, world_mpos(), world_mdelt());

	/**
	 * TODO: remove ALL of this into engine
	 */

	state.lcam.frame.y = state.lcam.frame.x / window.aspect;	/* TODO: make automatic */
	if (kb[GLFW_KEY_ESCAPE].down) this->close();
	// if (kb[GLFW_KEY_W].down) state.lcam.pos.y += dt * (4.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	// if (kb[GLFW_KEY_A].down) state.lcam.pos.x -= dt * (4.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	// if (kb[GLFW_KEY_S].down) state.lcam.pos.y -= dt * (4.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	// if (kb[GLFW_KEY_D].down) state.lcam.pos.x += dt * (4.f + ((kb[GLFW_KEY_LEFT_SHIFT].down) * 32.f));
	auto& ppos = state.world.ecs.getComp<c_Object>(player_system_get_player()).pos;
	state.lcam.pos += 0.1f * (ppos - state.lcam.pos);
	state.world.relocate(state.lcam.pos);

	if (abs(mouse.scroll.y) > 0.1) {
#ifdef __APPLE__
		float vwadd = dt * mouse.scroll.y * 100.f;
#else
		float vwadd = dt * mouse.scroll.y * 1000.f;
#endif
		local_cam_setvw(state.lcam, glm::clamp(state.lcam.frame.x + vwadd, 8.f, static_cast<float>(REGION_SIZE*(WORLD_DIAMETER-1))));
		// local_cam_setvw(state.lcam, state.lcam.frame.x + vwadd);
		// LOG_INF("changing vw to %.1f,%.1f", lcam.frame.x, lcam.frame.y);
	}


	// ugh TODO fix mouse
	if (mouse.left.down) {
		vec2 ssm = world_mpos();
		Tile& tile = state.world.tile_at(vec2(ssm.x, ssm.y));
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

	static size_t fpt = 0;
	fpt++;
	if (!(fpt % 1024)) LOG_INF("fps: %.1f", 1./dt);
}

void WorldDriver::user_render() {
	// input data from engine into buf renderer
	(*(brenderer.input_ptr())) = {
		.world = &state.world,
		.wmpos = world_mpos(),
		.lcam = this->state.lcam
	};
	// buffer render to framebuffer
	brenderer.prepare();
	brenderer.render();
	// transfer buf render output to post render input
	brenderer.write_output(prenderer.input_ptr());
	// world frame to window
	prenderer.prepare();
	prenderer.render();

	text_renderer.render(24, window.frame.y - 48, 2 * window.frame_to_window);
}

void WorldDriver::user_destroy() {
	engine.destroy(&state);
	gl.destroy();
}
















static WorldDriver driver;
int main() {
	driver.start();
	return 0;
}

