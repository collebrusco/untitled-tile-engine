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
    target = reg; target->raise_sflag();
}

void ShadowRenderer::use_world(World& w) {
    world = &w;
}

void ShadowRenderer::give_data(Camera& cam, vec2 lightpos) {
    current_shader->uMat4("uView", cam.view());
    current_shader->uMat4("uProj", cam.proj());
    current_shader->uVec2("ulpos", lightpos);
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

#define TOP_BIT (0x08)
#define BOT_BIT (0x04)
#define LFT_BIT (0x02)
#define RGT_BIT (0x01)

typedef union {
    struct {
        bool top    : 1;
        bool bot    : 1;
        bool l      : 1;
        bool r      : 1;
    } f;
    uint8_t val;
} surroundings_t;

static surroundings_t get_tile_surr(int i, int j, Region* tar, World* world) {
#ifdef __APPLE__
    surroundings_t surr = {
        .f.top = (j+1 > REGION_SIZE-1 ?
            world->read_region_at(region_coords_t(tar->pos.x, tar->pos.y+1))
                .buffer[i]
        :
            tar->buffer[i+(j+1)*REGION_SIZE]).surf.props.f.blocks_light,
        .f.bot = (j-1 < 0 ?
            world->read_region_at(region_coords_t(tar->pos.x, tar->pos.y-1))
                .buffer[i+((REGION_SIZE-1)*REGION_SIZE)]
        :
            tar->buffer[i+(j-1)*REGION_SIZE]).surf.props.f.blocks_light,
        .f.l = (i-1 < 0 ?
            world->read_region_at(region_coords_t(tar->pos.x-1, tar->pos.y))
                .buffer[REGION_SIZE-1+(j*REGION_SIZE)]
        :
            tar->buffer[i-1+j*REGION_SIZE]).surf.props.f.blocks_light,
        .f.r = (i+1 > REGION_SIZE-1 ?
            world->read_region_at(region_coords_t(tar->pos.x+1, tar->pos.y))
                .buffer[(j*REGION_SIZE)]
        :
            tar->buffer[i+1+j*REGION_SIZE]).surf.props.f.blocks_light
    };
#else
    surroundings_t surr;
    surr.f.top = (j+1 > REGION_SIZE-1 ?
        world->read_region_at(region_coords_t(tar->pos.x, tar->pos.y+1))
            .buffer[i]
    :
        tar->buffer[i+(j+1)*REGION_SIZE]).surf.props.f.blocks_light;
    surr.f.bot = (j-1 < 0 ?
        world->read_region_at(region_coords_t(tar->pos.x, tar->pos.y-1))
            .buffer[i+((REGION_SIZE-1)*REGION_SIZE)]
    :
        tar->buffer[i+(j-1)*REGION_SIZE]).surf.props.f.blocks_light;
    surr.f.l = (i-1 < 0 ?
        world->read_region_at(region_coords_t(tar->pos.x-1, tar->pos.y))
            .buffer[REGION_SIZE-1+(j*REGION_SIZE)]
    :
        tar->buffer[i-1+j*REGION_SIZE]).surf.props.f.blocks_light;
    surr.f.r = (i+1 > REGION_SIZE-1 ?
        world->read_region_at(region_coords_t(tar->pos.x+1, tar->pos.y))
            .buffer[(j*REGION_SIZE)]
    :
        tar->buffer[i+1+j*REGION_SIZE]).surf.props.f.blocks_light;
#endif
    return surr;
}

#define vpb shvs.push_back
#define ipb shis.push_back

void ShadowRenderer::prepare() {
    if (!target->read_sflag()) return;

    vector<uint32_t> shis; vector<Vt_2p2n> shvs;
    uint32_t shibase = 0;
    for (int64_t j = 0; j < REGION_SIZE; j++) {
        for (int64_t i = 0; i < REGION_SIZE; i++) {
            Tile& t = target->buffer[i+j*REGION_SIZE];
            bool surf = t.surf.props.f.present;
            float x = i; float y = j;
            tile_sprite_t img = surf ? t.surf.img : t.terr.img;
            if (!surf) continue;
            if (!t.surf.props.f.blocks_light) continue;
            surroundings_t surr = get_tile_surr(i, j, target, world);
            if (j == REGION_SIZE-1) world->region_at(ivec2(target->pos.x, target->pos.y+1));
            if (j == 0) world->region_at(ivec2(target->pos.x, target->pos.y-1));
            if (i == 0) world->region_at(ivec2(target->pos.x-1, target->pos.y));
            if (i == REGION_SIZE-1) world->region_at(ivec2(target->pos.x+1, target->pos.y));
            if (!surr.f.top) {
                vpb({{x   , y+1.}, {0., 0.}});
                vpb({{x   , y+1.}, {0., 1.}});
                vpb({{x+1., y+1.}, {0., 1.}});
                vpb({{x+1., y+1.}, {0., 0.}});

                ipb(shibase + 0); ipb(shibase + 2); ipb(shibase + 1);
                ipb(shibase + 0); ipb(shibase + 2); ipb(shibase + 3);
                shibase += 4;
            } else {
            }
            if (!surr.f.bot) {
                vpb({{x   , y}, {0.,  0.}});
                vpb({{x   , y}, {0., -1.}});
                vpb({{x+1., y}, {0., -1.}});
                vpb({{x+1., y}, {0.,  0.}});

                ipb(shibase + 0); ipb(shibase + 2); ipb(shibase + 1);
                ipb(shibase + 0); ipb(shibase + 2); ipb(shibase + 3);
                shibase += 4;
            } else {
            }
            if (!surr.f.l) {
                vpb({{x, y},    { 0.,  0.}});
                vpb({{x, y},    {-1.,  0.}});
                vpb({{x, y+1.}, {-1.,  0.}});
                vpb({{x, y+1.}, { 0.,  0.}});

                ipb(shibase + 0); ipb(shibase + 2); ipb(shibase + 1);
                ipb(shibase + 0); ipb(shibase + 2); ipb(shibase + 3);
                shibase += 4;
            } else {
            }
            if (!surr.f.r) {
                vpb({{x+1, y},    { 0.,  0.}});
                vpb({{x+1, y},    {+1.,  0.}});
                vpb({{x+1, y+1.}, {+1.,  0.}});
                vpb({{x+1, y+1.}, { 0.,  0.}});

                ipb(shibase + 0); ipb(shibase + 2); ipb(shibase + 1);
                ipb(shibase + 0); ipb(shibase + 2); ipb(shibase + 3);
                shibase += 4;
            } else {
            }
        }
    }

    target->clear_sflag();

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

