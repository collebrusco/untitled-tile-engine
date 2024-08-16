#include "config.h"
#ifdef BENCHMARK
	#include "util/debug_buf.h"
#endif
#include "CoreDriver.h"
#include "game/Followers.h"
#include "data/Animation.h"
#include "data/GenMesh.h"
#include <flgl/logger.h>
#include <iostream>
#include <vector>
LOG_MODULE(core);
using namespace glm;
using namespace std;

CoreDriver::CoreDriver() : GameDriver()
							{
							}
// static entID tid;
void CoreDriver::user_create() {
	/* gfx & window open */
	gl.init(); window.create("untitled", 980, 720);
	/* cam */
	state.cam.lcam.frame = vec2(REGION_SIZE*2.f, (REGION_SIZE*2.f)/window.aspect);
	lcam_control.spawn(state);
	/* spawn player & cam follow it */
	entID pid = Player::spawn(&state.world, {0.f,0.f}).id;
	lcam_control.follow(state, pid, 0.2f);

	// tid = state.world.newEntity();
	// state.world.addComp<c_Object>(tid).scale = {1.f,1.f};
	// state.world.addComp<c_GenMesh>(tid).emplace<HumanoidMesh>().walkin = true;
	
	/* RENDERING */
	(*(brenderer.input_ptr())) = {
		.world = &state.world,
		.wmpos = world_mpos(),
		.lcam = this->state.cam.lcam
	};
	brenderer.init();
	prenderer.init();
	
	text_renderer.init_text_rendering();
	text_renderer.init();
	text_renderer.set_text("move with wasd | scroll to zoom | click to place walls%s","");//\npress L to turn on lighting (bad) (wip) %s", "");
}

vec2 CoreDriver::world_mpos() const {
	vec2 res = vec2(window.mouse.pos / (vec2)window.frame);
	res.y = 1.f - res.y;
	res = res * state.cam.lcam.frame; /* tile coords relative to origin */
	res += state.cam.lcam.pos - (state.cam.lcam.frame/2.f);
	return res;
}

vec2 CoreDriver::world_mdelt() const {
	vec2 del = vec2(window.mouse.delta / (vec2)window.frame);
	del.y = 1.f - del.y;
	del = del * state.cam.lcam.frame; /* tile coords */
	return del;
}

void CoreDriver::user_update(float dt, Keyboard const& kb, Mouse const& mouse) {
	if (kb[GLFW_KEY_ESCAPE].down) this->close();

	// ((HumanoidMesh*)(&(state.world.getComp<c_GenMesh>(tid).get())))->walkin = kb[GLFW_KEY_0].down;

    lcam_control.update(state.cam.lcam, state.cam.e, state.world, dt);
	
	c_Actor::take_all_turns(state, kb, {.mouse = &mouse, .pos = world_mpos(), .delt = world_mdelt()});

	for (auto* action : state.actions) {
		action->perform();
		if (action->complete()) {
			state.actions.remove(action);
		}
	}

	c_DiffFollower::execute(state.world);

	c_Move::execute_moves(dt, &state.world);

	c_AnimationState::execute(dt, state.world);

	c_GenMesh::execute(dt, state.world);
}

void CoreDriver::user_render() {
	/* input data from engine into buf renderer */
	(*(brenderer.input_ptr())) = {
		.world = &state.world,
		.wmpos = world_mpos(),
		.lcam = this->state.cam.lcam
	};
	/* buffer render to framebuffer */
#ifdef BENCHMARK 
static Stopwatch sw(MICROSECONDS); sw.reset_start(); static debug_buf<float, 5> dbuf0, dbuf1, dbuf2;
#endif
	brenderer.prepare();
	brenderer.render();
#ifdef BENCHMARK
dbuf0.push(sw.stop());
if (dbuf0.ready()) {LOG_INF("\tfbuf rend %.1fus", dbuf0.get());}
sw.reset_start();
#endif
	/* transfer buf render output to post render input */
	brenderer.write_output(prenderer.input_ptr());
	/* world frame to window */
	prenderer.prepare();
	prenderer.render();
#ifdef BENCHMARK
dbuf1.push(sw.stop());
if (dbuf1.ready()) {LOG_INF("\tpost rend %.1fus", dbuf1.get());}
sw.reset_start();
#endif

	text_renderer.render(24, window.frame.y - 48, 2 * window.frame_to_window);
#ifdef BENCHMARK
dbuf2.push(sw.stop());
if (dbuf2.ready()) {LOG_INF("\ttext rend %.1fus", dbuf2.get());}
sw.reset_start();
#endif
}

void CoreDriver::user_destroy() {
	gl.destroy();
}

#ifdef UNITTEST
	DRIVER_MAIN_FUNCTION(app_main, CoreDriver);
#else
	DRIVER_MAIN_FUNCTION(main, CoreDriver);
#endif
