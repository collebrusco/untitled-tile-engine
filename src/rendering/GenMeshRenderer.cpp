#include "GenMeshRenderer.h"
#include "data/components.h"
#include "data/Animation.h"
#include "data/MeshAnimStack.h"
#include <flgl/glm.h>
#include <flgl/logger.h>
LOG_MODULE(gmrend);
using namespace glm;

/**
 * {{-1.,-1.}, {0.,0.}}
 * {{-1., 1.}, {0.,1.}}
 * {{ 1., 1.}, {1.,1.}}
 * {{ 1.,-1.}, {1.,0.}}
 * 
 * {0, 2, 1,	0, 2, 3}
 */

void GenMeshRenderer::init() {
    atlas_shader = Shader::from_source("mvp_vert", "color");

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

void GenMeshRenderer::prepare(ECS* ecs, Camera* cam) {
    this->ecs = ecs;
    /* sync viewproj */
    atlas_shader.bind();
    atlas_shader.uMat4("uView", cam->view());
    atlas_shader.uMat4("uProj", cam->proj());
}

void GenMeshRenderer::destroy() {
    atlas_shader.destroy();

    vao.destroy();
    vbo.destroy();
    ibo.destroy();
}

void GenMeshRenderer::render(Texture tile_tex) {
    mat4 model;
    tile_tex.bind();
    vao.unbind();
    for (auto e : ecs->view<c_Object, c_GenMesh>()) {
        auto& obj = ecs->getComp<c_Object>(e);
        auto& gm = ecs->getComp<c_GenMesh>(e);
        auto  pos = obj.pos;
        float rot = obj.rot;

        model = genModelMat2d(pos, rot, obj.scale, obj.anc);

        gm.get().sync(vbo, ibo);

        atlas_shader.bind();
        atlas_shader.uMat4("uModel", model);

        vao.bind();
        gl.draw_vao_ibo(ibo);
    }
    tile_tex.unbind();
    vao.unbind();
}
