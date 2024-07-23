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

void frame_vao_t::prepare(vec2 campos, vec2 lframe, ivec2 blpos, ivec2 framewh) {
    campos = campos - (vec2)blpos;
    // convert to 0,1 uv space
    campos /= (vec2)(framewh);
    campos += 0.5f;
    // LOG_INF("cpos %.2f,%.2f", campos.x, campos.y);
    lframe /= framewh;
    // LOG_INF("lfr %.2f,%.2f", lframe.x, lframe.y);
    // buffer
    uvbuf.bind();
    uvbuf.buffer<vec2>(
        {
            campos + vec2(-(lframe.x/2.f), -(lframe.y/2.f)),
            campos + vec2(-(lframe.x/2.f),  (lframe.y/2.f)),
            campos + vec2( (lframe.x/2.f),  (lframe.y/2.f)),
            campos + vec2( (lframe.x/2.f), -(lframe.y/2.f))
        }
    );
            vec2 a = campos + vec2(-(lframe.x/2.f), -(lframe.y/2.f));
            vec2 b = campos + vec2(-(lframe.x/2.f),  (lframe.y/2.f));
            vec2 c = campos + vec2( (lframe.x/2.f),  (lframe.y/2.f));
            vec2 d = campos + vec2( (lframe.x/2.f), -(lframe.y/2.f));
            // LOG_INF("%.4f,%.4f", a.x, a.y);
            // LOG_INF("%.4f,%.4f", b.x, b.y);
            // LOG_INF("%.4f,%.4f", c.x, c.y);
            // LOG_INF("%.4f,%.4f", d.x, d.y);
            // LOG_INF("================");
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
    frame_vao.prepare(details.local_cam.pos, details.local_cam.frame,
                      details.world_blpos, details.frame_pix);
}

void PostRenderer::render() {
    glDisable(GL_STENCIL_TEST);
    Framebuffer::bind_default();
    gl.viewport(window.frame.x, window.frame.y);
    glClear(GL_COLOR_BUFFER_BIT);
    // testq.bind();
    frame_vao.vao.bind();
    details.fbtex.bind();
    post_shader.bind();
    post_shader.uInt("utex", 0);
    // static bool l = 0;
    // if (window.keyboard[GLFW_KEY_L].pressed) l = !l;
    // post_shader.uFloat("ulightsw", l);
    gl.draw_vao_ibo(frame_vao.vao, frame_vao.ibo);
}

void PostRenderer::destroy() {
    post_shader.destroy();
    frame_vao.destroy();
    testq.destroy();
}




