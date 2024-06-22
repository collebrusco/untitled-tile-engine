#include "RegionRenderer.h"
#include <vector>
#include <flgl/logger.h>
using std::vector;
using namespace glm;
LOG_MODULE(rrend);

Texture RegionRenderer::tile_tex;
VertexArray RegionRenderer::t_vao;
VertexBuffer<vec3> RegionRenderer::t_posbuff;
ElementBuffer RegionRenderer::t_ibo;
Shader RegionRenderer::region_shader;

RegionRenderer::RegionRenderer() : target(0) {}


void RegionRenderer::use_region(Region* reg) {
    target = reg; target->raise_flag();
}

void RegionRenderer::sync_camera(Camera& cam) {
    region_shader.bind();
    region_shader.uMat4("uView", cam.view());
    region_shader.uMat4("uProj", cam.proj());
}

void RegionRenderer::static_init() {
    tile_tex = Texture::from_file("spritesheet");
    tile_tex.pixelate();
    region_shader = Shader::from_source("mvp_vert", "region_frag");
    t_vao.create(); t_vao.bind();
    t_posbuff.create(); t_posbuff.bind();
    t_ibo.create(); t_ibo.bind();
    t_vao.attrib(0, 3, GL_FLOAT, 0, 0);
    {
        vector<vec3> poss;
        vector<uint32_t> elems; 
        uint32_t idx = 0;
        for (size_t j = 0; j < REGION_SIZE; j++) {
            for (size_t i = 0; i < REGION_SIZE; i++) {
                float x = i, y = j;
                poss.push_back({x  ,y  ,0.f});
                poss.push_back({x  ,y+1,0.f});
                poss.push_back({x+1,y+1,0.f});
                poss.push_back({x+1,y  ,0.f});

                elems.push_back(idx);
                elems.push_back(idx+2);
                elems.push_back(idx+1);
                elems.push_back(idx);
                elems.push_back(idx+2);
                elems.push_back(idx+3);
                idx += 4;
            }
        }
        t_posbuff.buffer(poss); t_ibo.buffer(elems);
    }
    t_vao.unbind(); t_posbuff.unbind(); t_ibo.unbind();
}
void RegionRenderer::static_destroy() {
    t_vao.destroy(); t_ibo.destroy();
    t_posbuff.destroy();
    tile_tex.destroy();
    region_shader.destroy();
}

void RegionRenderer::init() {
    t_uvbuff.create();
}

static void uvpushback(vector<vec2>& uvs, sprite_t img) {
    const double tsz = 1. / (32.);
    double x = ((double)(img % TILE_SPRITESHEET_DIM)) / TILE_SPRITESHEET_DIM;
    double y = ((double)(img / TILE_SPRITESHEET_DIM)) / TILE_SPRITESHEET_DIM;
    uvs.push_back({x    +0.00001,y+tsz-0.00001});
    uvs.push_back({x    +0.00001,y    +0.00001});
    uvs.push_back({x+tsz-0.00001,y    +0.00001});
    uvs.push_back({x+tsz-0.00001,y+tsz-0.00001});
}

void RegionRenderer::prepare() {
    if (!target->read_flag()) return;

    vector<vec2> uvs;
    uvs.reserve(REGION_SIZE*REGION_SIZE*4);
    uint32_t shibase = 0;
    for (size_t j = 0; j < REGION_SIZE; j++) {
        for (size_t i = 0; i < REGION_SIZE; i++) {
            Tile& t = target->buffer[i+j*REGION_SIZE];
            bool surf = t.surf.props.f.present;
            sprite_t img = surf ? t.surf.img : t.terr.img;
            uvpushback(uvs, img);
        }
    }

    t_uvbuff.bind(); 
    t_uvbuff.buffer(uvs); 
    t_uvbuff.unbind();
}

void RegionRenderer::render() {
    mat4 model = genModelMat2d({target->pos.x * ((float)REGION_SIZE), target->pos.y * ((float)REGION_SIZE)}, 0.f, {1.f,1.f});
    region_shader.bind();
    region_shader.uMat4("uModel", model);
    tile_tex.bind();
    t_vao.bind();
    t_uvbuff.bind(); 
    t_vao.attrib(1, 2, GL_FLOAT, 0, 0);
    gl.draw_vao_ibo(t_vao, t_ibo);
    t_vao.unbind();
    tile_tex.unbind();
    t_uvbuff.unbind();
    t_posbuff.unbind();
}

void RegionRenderer::destroy() {
    t_uvbuff.destroy();
}

