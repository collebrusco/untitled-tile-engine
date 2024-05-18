#include <flgl.h>
#include <flgl/tools.h>
#include <flgl/glm.h>
using namespace glm;
#include "Driver.h"

#include <iostream>
using namespace std;


class WorldDriver : public GameDriver {
public:
	WorldDriver() : GameDriver() {}
private:
    virtual void user_create() override final;
    virtual void user_update(size_t tick, float dt) override final;
    virtual void user_render() override final;
    virtual void user_destroy() override final;
};


struct Renderer {
	virtual void init() = 0;
	virtual void prepare() = 0;
	virtual void render() = 0;
	virtual void destroy() = 0;
	static void clear() const {gl.clear();}
};

struct Tile {
	vec3 color;
};

#define WSIZE (64)
struct World {
	Tile tiles[WSIZE*WSIZE];
};




// renderer
static Mesh<Vt_classic> quad;
static Shader gridSh;

void WorldDriver::user_create() {
	{ // renderer
		gl.init();
		window.create("untitled", 1280, 720);
		quad = DefaultMeshes::tile<Vt_classic>();
		gridSh = Shader::from_source("fullscreenv", "gridf");
	}
}

void WorldDriver::user_update(size_t tick, float dt) {
	if (window.keyboard[GLFW_KEY_ESCAPE].down) this->close();

}

void WorldDriver::user_render() {
	{ // renderer
		gl.clear();
		gridSh.bind();
		gridSh.uFloat("uVW", 16.);
		gridSh.uFloat("uAspect", window.aspect);
		gridSh.uVec2("uMP", vec2{window.mouse.pos.x / (float)window.frame.x, ((window.frame.y - window.mouse.pos.y) / (float)window.frame.x)});
		quad.bind();
		gl.draw_mesh(quad);
	}
}

void WorldDriver::user_destroy() {
	{ // renderer
		gl.destroy();
	}
}
















static WorldDriver driver;
int main() {
	driver.create();
	driver.start();
	return 0;
}

