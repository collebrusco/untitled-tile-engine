/** 
 * EntityRenderer.h
 * created 06/24/24 by frank collebrusco 
 */
#ifndef ENTITY_RENDERER_H
#define ENTITY_RENDERER_H
#include <flgl.h>
#include <flgl/tools.h>
#include "ECS.h"

struct EntityRenderer {
    void init();
    void prepare(ECS* ecs, Camera* cam);
    void render(Texture tile_tex);
    void destroy();
private:
    ECS* ecs;
    VertexArray vao;
    VertexBuffer<Vt_classic> vbo;
    ElementBuffer ibo;
    Shader atlas_shader;
};

#endif /* ENTITY_RENDERER_H */
