#include "ShadowRenderer.h"
#include <vector>
#include <flgl/logger.h>
using std::vector;
using namespace glm;
LOG_MODULE(srend);

Shader ShadowRenderer::shadow_shader;
Shader ShadowRenderer::base_shader;
Shader* ShadowRenderer::current_shader = 0;

ShadowRenderer::ShadowRenderer() : target(0) {}


void ShadowRenderer::use_region(Region* reg) {
    target = reg; target->raise_flag();
}

void ShadowRenderer::use_world(World& w) {
    world = &w;
}

void ShadowRenderer::sync_camera(Camera& cam) {
    current_shader->uMat4("uView", cam.view());
    current_shader->uMat4("uProj", cam.proj());
    current_shader->uVec2("ulpos", cam.readPos().xy());
}

void ShadowRenderer::use_shader(Shader& sh) {
    current_shader = &sh;
}

void ShadowRenderer::static_init() {
    shadow_shader = Shader::from_source("shad_vert", "shad_frag");
    base_shader = Shader::from_source("mvp_vert", "shad_frag");
}
void ShadowRenderer::static_destroy() {
    shadow_shader.destroy();
    base_shader.destroy();
}

void ShadowRenderer::init() {
    sh_vao.create();
    sh_vbo.create();
    sh_ibo.create();
    
    sh_vao.bind();
    sh_vbo.bind();
    sh_vao.attach(sh_vbo);
    sh_ibo.bind();
    sh_vao.unbind();
}

#define vpb vs.push_back
#define ipb is.push_back

struct surroundings {
    bool top    : 1;
    bool bot    : 1;
    bool l      : 1;
    bool r      : 1;
};

static void shadowpushback(vector<Vt_pn>& vs, 
            vector<uint32_t>& is, size_t i, size_t j, uint32_t& shb,
            surroundings surr) {
    uint32_t oldvs = vs.size();
    vpb({{i+0.,  j+1., 0.}, {0,  1, 0}});
    vpb({{i+1.,  j+1., 0.}, {0,  1, 0}}); // sh
    vpb({{i+1.,  j+1., 0.}, {1,  0, 0}});
    vpb({{i+1.,  j+0., 0.}, {1,  0, 0}}); // sh
    vpb({{i+1.,  j+0., 0.}, {0, -1, 0}});
    vpb({{i+0.,  j+0., 0.}, {0, -1, 0}}); // sh
    vpb({{i+0.,  j+0., 0.}, {-1, 0, 0}});
    vpb({{i+0.,  j+1., 0.}, {-1, 0, 0}}); // sh
    uint32_t shbase = shb; shb += vs.size() - oldvs;
    ipb(shbase + 0); ipb(shbase + 1); ipb(shbase + 2);
    ipb(shbase + 2); ipb(shbase + 3); ipb(shbase + 4);
    ipb(shbase + 4); ipb(shbase + 5); ipb(shbase + 6);	
    ipb(shbase + 6); ipb(shbase + 7); ipb(shbase + 0);
    ipb(shbase + 0); ipb(shbase + 2); ipb(shbase + 6); 	
    ipb(shbase + 6); ipb(shbase + 2); ipb(shbase + 4);
}

void ShadowRenderer::prepare() {
    if (!target->read_flag()) return;

    vector<uint32_t> shis; vector<Vt_pn> shvs;
    uint32_t shibase = 0;
    for (int64_t j = 0; j < REGION_SIZE; j++) {
        for (int64_t i = 0; i < REGION_SIZE; i++) {
            Tile& t = target->buffer[i+j*REGION_SIZE];
            bool surf = t.surf.props.f.present;
            sprite_t img = surf ? t.surf.img : t.terr.img;
            if (!surf) continue;
            if (!t.surf.props.f.blocks_light) continue;
            surroundings surr = {
                .top = (j+1 > REGION_SIZE-1 ?
                    world->read_region_at(region_coords_t(target->pos.x, target->pos.y+1))
                        .buffer[i]
                :
                    target->buffer[i+(j+1)*REGION_SIZE]).surf.props.f.blocks_light,
                .bot = (j-1 < 0 ?
                    world->read_region_at(region_coords_t(target->pos.x, target->pos.y-1))
                        .buffer[i+((REGION_SIZE-1)*REGION_SIZE)]
                :
                    target->buffer[i+(j-1)*REGION_SIZE]).surf.props.f.blocks_light,
                .l = (i-1 < 0 ?
                    world->read_region_at(region_coords_t(target->pos.x-1, target->pos.y))
                        .buffer[REGION_SIZE-1+(j*REGION_SIZE)]
                :
                    target->buffer[i-1+j*REGION_SIZE]).surf.props.f.blocks_light,
                .r = (i+1 > REGION_SIZE-1 ?
                    world->read_region_at(region_coords_t(target->pos.x+1, target->pos.y))
                        .buffer[(j*REGION_SIZE)]
                :
                    target->buffer[i+1+j*REGION_SIZE]).surf.props.f.blocks_light
            };
            shadowpushback(shvs, shis, i, j, shibase, surr);
        }
    }

    sh_vbo.bind();
    sh_vbo.buffer(shvs);
    sh_vbo.unbind();
    sh_ibo.bind();
    sh_ibo.buffer(shis);
    sh_ibo.unbind();
}

void ShadowRenderer::render() { 
    // TODO optimize: recalcing mat for reg shadow pair :(
    mat4 model = genModelMat2d({target->pos.x * ((float)REGION_SIZE), target->pos.y * ((float)REGION_SIZE)}, 0.f, {1.f,1.f});

    current_shader->bind();
    current_shader->uMat4("uModel", model);
    sh_vao.bind();
    gl.draw_vao_ibo(sh_vao, sh_ibo);
    sh_vao.unbind();

}

void ShadowRenderer::destroy() {
    sh_vao.destroy();
    sh_vbo.destroy();
    sh_ibo.destroy();
}

