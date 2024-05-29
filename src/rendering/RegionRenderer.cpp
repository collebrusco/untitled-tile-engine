#include "RegionRenderer.h"
#include <vector>
#include <flgl/logger.h>
using std::vector;
using namespace glm;
LOG_MODULE(rrend);

Texture RegionRenderer::tile_tex;
VertexArray RegionRenderer::vao;
VertexBuffer<vec3> RegionRenderer::posbuff;
ElementBuffer RegionRenderer::ibo;
Shader RegionRenderer::region_shader;

RegionRenderer::RegionRenderer() : target(0) {}


void RegionRenderer::use_region(Region* reg) {
    target = reg; target->raise_flag();
}

void RegionRenderer::sync_shader(Camera& cam) {
    region_shader.uMat4("uView", cam.view());
    region_shader.uMat4("uProj", cam.proj());
}

void RegionRenderer::static_init() {
    tile_tex = Texture::from_file("spritesheet");
    tile_tex.pixelate();
    region_shader = Shader::from_source("mvp_vert", "region_frag");
    vao.create(); vao.bind();
    posbuff.create(); posbuff.bind();
    ibo.create(); ibo.bind();
    vao.attrib(0, 3, GL_FLOAT, 0, 0);
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
        posbuff.buffer(poss); ibo.buffer(elems);
    }
    vao.unbind(); posbuff.unbind(); ibo.unbind();
}
void RegionRenderer::static_destroy() {
    vao.destroy(); ibo.destroy();
    posbuff.destroy();
    tile_tex.destroy();
    region_shader.destroy();
}

void RegionRenderer::init() {
    uvbuff.create();
}

void RegionRenderer::prepare() {
    if (!target->read_flag()) return;
    vector<vec2> uvs;
    const double tsz = 1. / (32.);
    for (size_t j = 0; j < REGION_SIZE; j++) {
        for (size_t i = 0; i < REGION_SIZE; i++) {
            Tile& t = target->buffer[i+j*REGION_SIZE];
            double x = ((double)(t.img % 32)) / 32.f;
            double y = ((double)(t.img / 32)) / 32.f;
            uvs.push_back({x    +0.00001,y+tsz-0.00001});
            uvs.push_back({x    +0.00001,y    +0.00001});
            uvs.push_back({x+tsz-0.00001,y    +0.00001});
            uvs.push_back({x+tsz-0.00001,y+tsz-0.00001});
        }
    }
    vao.bind();
    uvbuff.bind(); 
    vao.attrib(1, 2, GL_FLOAT, 0, 0);
    uvbuff.buffer(uvs); 
    uvbuff.unbind();
    vao.unbind();
    target->clear_flag();
    // LOG_DBG("reloaded %d,%d", target->pos.x, target->pos.y);
}

void RegionRenderer::render() {
    mat4 model = genModelMat2d({target->pos.x * ((float)REGION_SIZE), target->pos.y * ((float)REGION_SIZE)}, 0.f, {1.f,1.f});
    region_shader.bind();
    region_shader.uMat4("uModel", model);
    tile_tex.bind();
    vao.bind();
    uvbuff.bind(); 
    vao.attrib(1, 2, GL_FLOAT, 0, 0);
    gl.draw_vao_ibo(vao, ibo);
}

void RegionRenderer::destroy() {
    uvbuff.destroy();
}

