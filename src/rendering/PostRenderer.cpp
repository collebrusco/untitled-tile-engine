#include "PostRenderer.h"
#include <flgl/logger.h>
LOG_MODULE(prend);
using namespace glm;

void frame_vao_t::create() {
    vao.create(); posbuf.create(); uvbuf.create(); ibo.create();
    vao.bind();
    posbuf.bind();
    vao.attrib(0, 2, GL_FLOAT, 0, 0);

    posbuf.buffer<vec2>({{-1.,-1.},
                         {-1., 1.},
                         { 1., 1.},
                         { 1.,-1.}});

    uvbuf.bind();
    vao.attrib(1, 2, GL_FLOAT, 0, 0);

    ibo.bind();
    ibo.buffer<uint32_t>({0, 2, 1,    0, 2, 3});

    vao.unbind();
    posbuf.unbind(); uvbuf.unbind(); ibo.unbind();
}

void frame_vao_t::destroy() {
    vao.destroy(); posbuf.destroy(); uvbuf.destroy();
}

void frame_vao_t::prepare(glm::vec2 campos, float vw, float asp, ivec2 center, ivec2 framewh) {
    // calc world pos of bot left tile of frame
    tile_coords_t wposbl = center - framewh/2;
    // use that to find cam pos relative to that tile
    campos = campos - (vec2)wposbl;
    // convert to 0,1 uv space
    campos /= (vec2)(framewh*REGION_SIZE);
    // buffer
    uvbuf.bind();
    uvbuf.buffer<vec2>(
        {
            campos + vec2(-(vw/2.f), -((vw/asp)/2.f)),
            campos + vec2(-(vw/2.f),  ((vw/asp)/2.f)),
            campos + vec2( (vw/2.f),  ((vw/asp)/2.f)),
            campos + vec2( (vw/2.f), -((vw/asp)/2.f))
        }
    );
    uvbuf.unbind();
}

/**
 * ================ RENDERER ================
 */

PostRenderer::PostRenderer() {}

void PostRenderer::twf() {

}

void PostRenderer::init() {
    post_shader = Shader::from_source("fullscreenv", "tex");
    frame_vao.create();
    testq = DefaultMeshes::tile<Vt_classic>();
}

void PostRenderer::prepare() {

}

void PostRenderer::render() {
    glDisable(GL_STENCIL_TEST);
    Framebuffer::bind_default();
    gl.viewport(window.frame.x, window.frame.y);
    glClear(GL_COLOR_BUFFER_BIT);
    testq.bind();
    details.fbtex.bind();
    post_shader.bind();
    post_shader.uInt("utex", 0);
    // post_shader.uFloat("uAspect", window.aspect);
    // post_shader.uVec2("uRes", window.frame);
    // post_shader.uFloat("uTime", timer.read());
    // post_shader.uVec2("uCampos", details.local_cam.pos);
    // post_shader.uFloat("uvw", details.local_cam.frame.x);
    // static bool l = 0;
    // if (window.keyboard[GLFW_KEY_L].pressed) l = !l;
    // post_shader.uFloat("ulightsw", l);
    testq.bind();
    gl.draw_mesh(testq);
}

void PostRenderer::destroy() {
    post_shader.destroy();
    frame_vao.destroy();
    testq.destroy();
}




