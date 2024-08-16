/** 
 * GenMeshRenderer.h
 * created 08/15/24 by frank collebrusco 
 */
#ifndef GEN_MESH_RENDERER_H
#define GEN_MESH_RENDERER_H
#include <flgl.h>
#include <flgl/tools.h>
#include "ECS.h"
#include "data/GenMesh.h"

struct GenMeshRenderer {
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


#endif /* GEN_MESH_RENDERER_H */
