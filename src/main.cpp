#include <flgl.h>
#include <flgl/tools.h>
#include <flgl/glm.h>
using namespace glm;
#include "Driver.h"
#include "Renderer.h"

#include <iostream>
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
};



// renderer
static Mesh<Vt_classic> quad;
static Shader gridSh;

WorldDriver::WorldDriver() : GameDriver() {}

void WorldDriver::user_create() {
	{ // renderer
		gl.init();
		window.create("untitled", 1280, 720);
		quad = DefaultMeshes::tile<Vt_classic>();
		gridSh = Shader::from_source("fullscreenv", "gridf");
	}
}

void WorldDriver::user_update(float dt, Keyboard const& kb, Mouse const& mouse) {
	if (kb[GLFW_KEY_ESCAPE].down) this->close();

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
	driver.start();
	return 0;
}

