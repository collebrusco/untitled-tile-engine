#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include <flgl.h>
#include <flgl/glm.h>
#include "RegionRenderer.h"
#include "ShadowRenderer.h"
#include "game/World.h"
#include <Stopwatch.h>

struct WorldRenderer : public Renderer {

    WorldRenderer() = default;
    virtual ~WorldRenderer() = default;

    void use_camera(OrthoCamera& c);
    void give_mouse(glm::ivec2 mp);
    void use_world(World& w);
    void twf();

    virtual void init() override final;
    virtual void prepare() override final;
    virtual void render() override final;
    virtual void destroy() override final;

private:
    Stopwatch timer;
    OrthoCamera* cam;
    World* world;
	RegionRenderer rrenderers[WORLD_DIAMETER*WORLD_DIAMETER];
	ShadowRenderer srenderers[WORLD_DIAMETER*WORLD_DIAMETER];

    Framebuffer fbuf;
    Texture fbtex;
    Renderbuffer fbrbuf;
    Shader quad_shader;
    Shader quad_perlin;
    Mesh<Vt_classic> quad;
    Mesh<glm::vec2> outline;
    Shader ol_shader;

    glm::ivec2 pframe;
    glm::ivec2 mpos;

    bool wf;
};


#endif
