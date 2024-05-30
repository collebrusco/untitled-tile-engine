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

	MapWSave wsave;
	World world;

	WorldRenderer wrenderer;
	
	OrthoCamera cam;

};

WorldDriver::WorldDriver() : GameDriver(), 	
							wsave(),
							world(wsave),
							cam({0.f,0.f,1.f}, {0.f, 0.f, -1.f}, {0.f, 1.f, 0.f}, 0.001f, 10000.f, 64)
							{
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

	static region_coords_t prpos;
	ivec2 cp = ((ivec2)cam.readPos()) / REGION_SIZE;
	if (prpos != cp) {
		ivec2 delta = (cp) - prpos;
		world.shift(delta.x, delta.y);
		prpos = cp;
	}

	if (kb[GLFW_KEY_O].pressed) world.shift(1,1);
	if (kb[GLFW_KEY_L].pressed) world.shift(1,-1);
	if (kb[GLFW_KEY_K].pressed) world.shift(-1,-1);
	if (kb[GLFW_KEY_I].pressed) world.shift(-1,1);

	{
		if (mouse.left.down) {
			static Tile ptile = {.img=1023}; static Tile const& prtile = ptile;
			vec4 ssm = {mouse.pos.x,mouse.pos.y,0.,1.};
			ssm.x /= window.frame.x; ssm.y /= window.frame.y;
			ssm.x *= 2.f; ssm.x -= 1.f;
			ssm.y *= 2.f; ssm.y = 2.f - ssm.y; ssm.y -= 1.f; 
			mat4 iv = inverse(cam.view()); mat4 ip = inverse(cam.proj());
			ssm = iv * (ip * ssm);
			Tile const& ctile = world.read_tile_at(ssm.xy());
			Tile& tile = world.tile_at(vec2(ssm.x, ssm.y));
			tile.img = 99;
		}
	}

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

