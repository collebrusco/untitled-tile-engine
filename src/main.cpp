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



#include <util/abstract_linear_objpool.h>
abstract_linear_objpool<System> alop(1024);
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
	if (kb[GLFW_KEY_ESCAPE].down) this->close();
	state.lcam.frame.y = state.lcam.frame.x / window.aspect;	/* TODO: make automatic */

	/**
	 * STEP ENGINE
	 * this currently just runs through the systems
	 */
	engine.step(dt, &state, kb, mouse, world_mpos(), world_mdelt());

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

