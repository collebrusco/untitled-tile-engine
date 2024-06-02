// 
// EntityRenderer.h
// created 06/01/24 by frank collebrusco 
//
#ifndef ENTITY_RENDERER
#define ENTITY_RENDERER
#include "Renderer.h"
#include "ECS.h"

struct EntityRenderer : public Renderer {
    explicit EntityRenderer(ECS& h);

    entID target;
    ECS* home;

    virtual void init() override;
    virtual void prepare() override;
    virtual void render() override;
    virtual void destroy() override;
};


#endif /* ENTITY_RENDERER */
