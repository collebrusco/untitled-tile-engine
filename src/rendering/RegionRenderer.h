#ifndef REGION_RENDERER_H
#define REGION_RENDERER_H
#include "../game/World.h"
#include <flgl.h>
#include <flgl/glm.h>

struct RegionRenderer {
	RegionRenderer();
	virtual ~RegionRenderer() = default;

	static VertexArray t_vao;
	static VertexBuffer<glm::vec3> t_posbuff;
	static ElementBuffer t_ibo;
	static Shader region_shader;
	VertexBuffer<glm::vec2> t_uvbuff;

	Region* target;

	void use_region(Region* reg);

	static void sync_camera(Camera& cam);

	static void static_init();
	static void static_destroy();

	void init();
	void prepare();
	void render(Texture tile_tex);
	void destroy();


};


#endif