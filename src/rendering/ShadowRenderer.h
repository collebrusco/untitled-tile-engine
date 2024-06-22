#ifndef SHADOW_RENDERER_H
#define SHADOW_RENDERER_H
#include "Renderer.h"
#include "render_pipeline_structs.h"
#include "../game/World.h"
#include <flgl.h>
#include <flgl/glm.h>

struct ShadowRenderer : public Renderer {
	ShadowRenderer();
	virtual ~ShadowRenderer() = default;

	static Shader shadow_shader;
	static Shader base_shader;
	static Shader* current_shader;

	VertexArray sh_vao;
	VertexBuffer<Vt_2p2n> sh_vbo;
	ElementBuffer sh_ibo;

	World* world;
	Region* target;

	void use_region(Region* reg);
	void use_world(World& w);

	static void give_data(Camera& cam, glm::vec2 lightpos);

	/* TODO obsolete unless I change stenciling again */
	static void use_shader(Shader& sh);

	static void static_init();
	static void static_destroy();

	virtual void init() override final;

	virtual void prepare() override final;

	virtual void render() override final;

	virtual void destroy() override final;


};


#endif