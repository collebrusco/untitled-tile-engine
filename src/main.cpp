#include <flgl.h>
#include <flgl/tools.h>
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

void WorldDriver::user_create() {
	{ // renderer
		gl.init();
		window.create("untitled", 682, 512);
	}
}

void WorldDriver::user_update(size_t tick, float dt) {
	if (window.keyboard[GLFW_KEY_ESCAPE].down) this->close();

}

void WorldDriver::user_render() {
	{ // renderer
		gl.clear();

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

