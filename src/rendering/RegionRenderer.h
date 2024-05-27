#ifndef REGION_RENDERER_H
#define REGION_RENDERER_H
#include "Renderer.h"
#include "../game/World.h"
#include <flgl.h>
#include <flgl/glm.h>

struct RegionRenderer : public Renderer {
	RegionRenderer(Region& r);

	static Texture tile_tex;
	static VertexArray vao;
	static VertexBuffer<glm::vec3> posbuff;
	static ElementBuffer ibo;
	static Shader region_shader;
	VertexBuffer<glm::vec2> uvbuff;

	Region* target;

	void new_region(Region* reg);

	static void sync_shader(Camera& cam);

	static void static_init();
	static void static_destroy();

	virtual void init() override final;

	virtual void prepare() override final;

	virtual void render() override final;

	virtual void destroy() override final;


};


#endif