#include "EntityRenderer.h"
#include "data/components.h"
#include <flgl/glm.h>
#include <flgl/logger.h>
LOG_MODULE(erend);
using namespace glm;

/**
 * {{-1.,-1.}, {0.,0.}}
 * {{-1., 1.}, {0.,1.}}
 * {{ 1., 1.}, {1.,1.}}
 * {{ 1.,-1.}, {1.,0.}}
 * 
 * {0, 2, 1,	0, 2, 3}
 */

void EntityRenderer::init() {
    atlas_shader = Shader::from_source("2Dmvp_vert", "entity_atlas_f");

    vao.create();
    vbo.create();
    ibo.create();

    vao.bind();
    vbo.bind();
    vao.attach(vbo);

    ibo.bind();
    ibo.buffer<uint32_t>({0, 2, 1,    0, 2, 3});

    vao.unbind();
    vbo.unbind();
    ibo.unbind();
}

void EntityRenderer::prepare(ECS* ecs, Camera* cam) {
    this->ecs = ecs;
    /* sync viewproj */
    atlas_shader.bind();
    atlas_shader.uMat4("uView", cam->view());
    atlas_shader.uMat4("uProj", cam->proj());
}

void EntityRenderer::render(Texture tile_tex) {
    mat4 model;
    Vt_2Dclassic verts[4];
    tile_tex.bind();
    for (auto e : ecs->view<c_Object, c_StaticAtlas>()) {
        auto& obj = ecs->getComp<c_Object>(e);
        auto& atl = ecs->getComp<c_StaticAtlas>(e);

        model = genModelMat2d(obj.pos, obj.rot, obj.scale, obj.anc);

        verts[0] = {{-0.5,-0.5}, {atl.uvs.bl.x, atl.uvs.bl.y}};
        verts[1] = {{-0.5, 0.5}, {atl.uvs.bl.x, atl.uvs.tr.y}};
        verts[2] = {{ 0.5, 0.5}, {atl.uvs.tr.x, atl.uvs.tr.y}};
        verts[3] = {{ 0.5,-0.5}, {atl.uvs.tr.x, atl.uvs.bl.y}};

        vbo.bind();
        vbo.buffer_data(4, verts);
        vbo.unbind();

        atlas_shader.bind();
        atlas_shader.uMat4("uModel", model);

        vao.bind();
        gl.draw_vao_ibo(ibo);
    }
    tile_tex.unbind();
}

void EntityRenderer::destroy() {
    atlas_shader.destroy();

    vao.destroy();
    vbo.destroy();
    ibo.destroy();
}


