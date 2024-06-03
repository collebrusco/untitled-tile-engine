#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include <flgl.h>
#include <flgl/glm.h>
#include "RegionRenderer.h"
#include "ShadowRenderer.h"
#include "game/World.h"

struct WorldRenderer : public Renderer {

    WorldRenderer() = default;
    virtual ~WorldRenderer() = default;

    void use_camera(OrthoCamera& c);
    void use_world(World& w);
    void twf();

    virtual void init() override final;
    virtual void prepare() override final;
    virtual void render() override final;
    virtual void destroy() override final;

private:
    OrthoCamera* cam;
    World* world;
	RegionRenderer rrenderers[WORLD_DIAMETER*WORLD_DIAMETER];
	ShadowRenderer srenderers[WORLD_DIAMETER*WORLD_DIAMETER];

    Framebuffer fbuf;
    Texture fbtex;
    Renderbuffer fbrbuf;
    glm::ivec2 pframe;
    Shader quad_shader;
    Mesh<Vt_classic> quad;

    bool wf;
};


#endif
