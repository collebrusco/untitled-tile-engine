#include "EntityRenderer.h"
#include "data/components.h"
#include "data/Animation.h"
#include "data/GenMesh.h"
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
    atlas_shader = Shader::from_source("mvp_vert", "entity_atlas_f");

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

void EntityRenderer::destroy() {
    atlas_shader.destroy();

    vao.destroy();
    vbo.destroy();
    ibo.destroy();
}

void EntityRenderer::render(Texture tile_tex) {
    mat4 model;
    Vt_classic verts[4];
    tile_tex.bind();
    glEnable(GL_DEPTH_TEST);
    VertexArray::unbind();
    /** Simple quad sprites */
    ibo.bind();
    ibo.buffer<uint32_t>({0, 2, 1,    0, 2, 3});
    ibo.unbind();
    for (auto e : ecs->view<c_Object, c_EntRenderEntry>()) {
        auto& obj = ecs->getComp<c_Object>(e);
        auto& atl = ecs->getComp<c_EntRenderEntry>(e);
        auto  pos = obj.pos;
        float rot = obj.rot;

        if (atl.props.pix_snap) {
            pos = (floor(pos * TILE_PIXELS_F)) / TILE_PIXELS_F;
        }
        int8_t hmirr = 1, vmirr = 1;
        switch (atl.props.rot) {
        case ROTATE_NONE:
            rot = 0;
            break;
        case ROTATE_HAXIS_MIRROR:
            if (rot > 90.f && rot < 270.f) hmirr = -1;
            rot = 0;
            break;
        case ROTATE_VAXIS_MIRROR:
            if (rot > 180.f) vmirr = -1;
            rot = 0;
            break;
        case ROTATE_2_PT:
            rot = floor(rot/180.f) * 180.f;
            break;
        case ROTATE_4_PT:
            rot = floor((rot+45.f)/90.f) * 90.f;
            break;
        case ROTATE_6_PT:
            rot = floor((rot+30.f)/60.f) * 60.f;
            break;
        case ROTATE_FULL:
        default:
            break;
        }

        model = genModelMat2d(pos, rot, obj.scale, obj.anc);

        verts[0] = {{(float)vmirr * -0.5f, (float)hmirr * -0.5f, 0.f}, {atl.uvs.bl.x, atl.uvs.bl.y}};
        verts[1] = {{(float)vmirr * -0.5f, (float)hmirr *  0.5f, 0.f}, {atl.uvs.bl.x, atl.uvs.tr.y}};
        verts[2] = {{(float)vmirr *  0.5f, (float)hmirr *  0.5f, 0.f}, {atl.uvs.tr.x, atl.uvs.tr.y}};
        verts[3] = {{(float)vmirr *  0.5f, (float)hmirr * -0.5f, 0.f}, {atl.uvs.tr.x, atl.uvs.bl.y}};

        vbo.bind();
        vbo.buffer_data(4, verts);
        vbo.unbind();

        atlas_shader.bind();
        atlas_shader.uMat4("uModel", model);

        vao.bind();
        gl.draw_vao_ibo(ibo);
        vao.unbind();
    }
    /** general mesh */
    // gl.wireframe(true);
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
        vao.unbind();
    }
    gl.wireframe(false);
    tile_tex.unbind();
    vao.unbind();
    glDisable(GL_DEPTH_TEST);
}
