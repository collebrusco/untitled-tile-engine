#include <flgl.h>
#include <flgl/tools.h>
#include <flgl/glm.h>
#include <flgl/logger.h>
#include "Driver.h"
#include "Renderer.h"
LOG_MODULE(main);

#include <iostream>
#include <vector>
using namespace glm;
using namespace std;
typedef unsigned int sprite_t;


struct Tile {
	bool solid;
	float friction;
	sprite_t img;
};

#define REGION_SIZE_LOG (6)
#define REGION_SIZE (1<<REGION_SIZE_LOG)

struct Region {
	Tile buffer[REGION_SIZE*REGION_SIZE];
	ivec2 reg_pos; // world pos * REGION_SIZE

	Tile& tile_at(int x, int y) {return buffer[x+y*REGION_SIZE];}

};

struct RegionRenderer : public Renderer {
	RegionRenderer(Region& r) : target(&r) {}

	static Texture tile_tex;
	static VertexArray vao;
	static VertexBuffer<vec3> posbuff;
	static ElementBuffer ibo;
	static Shader region_shader;
	VertexBuffer<vec2> uvbuff;

	Region* target;

	void new_region(Region* reg) {
		target = reg;
	}

	static void sync_shader(Camera& cam) {
		region_shader.uMat4("uView", cam.view());
		region_shader.uMat4("uProj", cam.proj());
	}

	static void static_init() {
		tile_tex = Texture::from_file("spritesheet");
		region_shader = Shader::from_source("mvp_vert", "region_frag");
		vao.create(); vao.bind();
		posbuff.create(); posbuff.bind();
		ibo.create(); ibo.bind();
		vao.attrib(0, 3, GL_FLOAT, 0, 0);
		{
			vector<vec3> poss;
			vector<uint32_t> elems; uint32_t idx = 0;
			for (size_t j = 0; j < REGION_SIZE; j++) {
				for (size_t i = 0; i < REGION_SIZE; i++) {
					float x = i, y = j;
					poss.push_back({x  ,y  ,0.f});
					poss.push_back({x  ,y+1,0.f});
					poss.push_back({x+1,y+1,0.f});
					poss.push_back({x  ,y  ,0.f});

					elems.push_back(idx);
					elems.push_back(idx+2);
					elems.push_back(idx+1);
					elems.push_back(idx);
					elems.push_back(idx+2);
					elems.push_back(idx+3);
					idx += 4;
				}
			}
			posbuff.buffer(poss); ibo.buffer(elems);
		}
		vao.unbind(); posbuff.unbind(); ibo.unbind();
	}
	static void static_destroy() {
		vao.destroy(); ibo.destroy();
		posbuff.destroy();
		tile_tex.destroy();
		region_shader.destroy();
	}

	virtual void init() override final {
		uvbuff.create();
	}

	virtual void prepare() override final {
		vector<vec2> uvs;
		const float tsz = 1. / (32.f);
		for (size_t j = 0; j < REGION_SIZE; j++) {
			for (size_t i = 0; i < REGION_SIZE; i++) {
				Tile& t = target->buffer[i+j*REGION_SIZE];
				float x = 1. / (float)(t.img % 32);
				float y = 1. / (float)(t.img / 32);
				uvs.push_back({x,y+tsz});
				uvs.push_back({x,y});
				uvs.push_back({x+tsz,y});
				uvs.push_back({x+tsz,y+tsz});
			}
		}
		uvbuff.bind(); uvbuff.buffer(uvs); uvbuff.unbind();
	}

	virtual void render() override final {
		mat4 model = genModelMat2d({target->reg_pos.x * 32.f, target->reg_pos.y * 32.f}, 0.f, {1.f,1.f});
		region_shader.bind();
		region_shader.uMat4("uModel", model);
		vao.bind();
		gl.draw_vao_ibo(vao, ibo);
	}

	virtual void destroy() override final {
		uvbuff.destroy();
	}


};

Texture RegionRenderer::tile_tex;
VertexArray RegionRenderer::vao;
VertexBuffer<vec3> RegionRenderer::posbuff;
ElementBuffer RegionRenderer::ibo;
Shader RegionRenderer::region_shader;











class WorldDriver : public GameDriver {
public:
	WorldDriver();
	virtual ~WorldDriver() = default;
private:
    virtual void user_create() override final;
    virtual void user_update(float dt, Keyboard const& kb, Mouse const& mouse) override final;
    virtual void user_render() override final;
    virtual void user_destroy() override final;

	Region testr;
	RegionRenderer renderer;
	OrthoCamera cam;

};

WorldDriver::WorldDriver() : GameDriver(), 	
							testr(), 
							renderer(testr), 
							cam({0.f,0.f,-1.f}, {0.f, 0.f, 1.f}, {0.f, 1.f, 0.f}, 0.001f, 10000.f, 64) {}

void WorldDriver::user_create() {
	testr = {
		.reg_pos = {0,0}
	};
	for (size_t i = 0; i < 1024; i++)
		testr.buffer[i].img = i+1;
	Renderer::context_init("untitled", 1280, 720);
	cam.update();
	renderer.static_init();
	renderer.init();
}

void WorldDriver::user_update(float dt, Keyboard const& kb, Mouse const& mouse) {
	if (kb[GLFW_KEY_ESCAPE].down) this->close();
	if (kb[GLFW_KEY_W].down) cam.getPos().y += dt * 2.f;
	if (kb[GLFW_KEY_A].down) cam.getPos().x -= dt * 2.f;
	if (kb[GLFW_KEY_S].down) cam.getPos().y -= dt * 2.f;
	if (kb[GLFW_KEY_D].down) cam.getPos().x += dt * 2.f;
	if (abs(mouse.scroll.y) > 0.1) cam.getViewWidth() += dt * mouse.scroll.y * 10.f;
	cam.update();

	static uint tg = 0; tg++;
	if (!(tg%64)) {
		LOG_DBG("fps: %.1f", 1.f/dt);
		LOG_DBG("cam pos: %.2f,%.2f", cam.readPos().x, cam.readPos().y);
	}

}

void WorldDriver::user_render() {
	Renderer::clear();
	renderer.prepare();
	renderer.sync_shader(cam);
	renderer.render();
	Renderer::frame_finish();
}

void WorldDriver::user_destroy() {
	Renderer::context_close();
}
















static WorldDriver driver;
int main() {
	driver.start();
	return 0;
}

