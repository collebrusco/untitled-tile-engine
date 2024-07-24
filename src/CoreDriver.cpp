#include "CoreDriver.h"
#include <flgl/logger.h>
LOG_MODULE(main);

#include <iostream>
#include <vector>
using namespace glm;
using namespace std;

#ifdef UNITTEST
	DRIVER_MAIN_FUNCTION(app_main, CoreDriver);
#else
	DRIVER_MAIN_FUNCTION(main, CoreDriver);
#endif

CoreDriver::CoreDriver() : GameDriver()
							{
							}

void CoreDriver::user_create() {
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

vec2 CoreDriver::world_mpos() const {
	vec2 res = vec2(window.mouse.pos / (vec2)window.frame);
	res.y = 1.f - res.y;
	res = res * state.lcam.frame; /* tile coords relative to origin */
	res += state.lcam.pos - (state.lcam.frame/2.f);
	return res;
}

vec2 CoreDriver::world_mdelt() const {
	vec2 del = vec2(window.mouse.delta / (vec2)window.frame);
	del.y = 1.f - del.y;
	del = del * state.lcam.frame; /* tile coords */
	return del;
}

void CoreDriver::user_update(float dt, Keyboard const& kb, Mouse const& mouse) {
	if (kb[GLFW_KEY_ESCAPE].down) this->close();
    lcam_control.update(state.lcam, dt);

}

void CoreDriver::user_render() {
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

void CoreDriver::user_destroy() {
	gl.destroy();
}

