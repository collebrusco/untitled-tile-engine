#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include <flgl.h>
#include "RegionRenderer.h"
#include "game/World.h"

struct WorldRenderer : public Renderer {

    WorldRenderer() = default;
    virtual ~WorldRenderer() = default;

    void use_camera(OrthoCamera& c);
    void use_world(World& w);

    virtual void init() override final;
    virtual void prepare() override final;
    virtual void render() override final;
    virtual void destroy() override final;

private:
    OrthoCamera* cam;
    World* world;
	RegionRenderer renderers[WORLD_DIAMETER*WORLD_DIAMETER];
};


#endif
