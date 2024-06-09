#include "WorldRenderer.h"
#include <flgl/logger.h>
#include <flgl/glm.h>
#include <flgl/tools.h>
LOG_MODULE(wrend);
using namespace glm;

void WorldRenderer::use_camera(OrthoCamera& c) {
    cam = &c;
}

void WorldRenderer::use_world(World& w) {
    world = &w;
}

void WorldRenderer::give_mouse(glm::ivec2 mp) {
    mpos = mp;
}

void WorldRenderer::twf() {wf = !wf;}

void WorldRenderer::init() {
    timer.setUnit(SECONDS);
    timer.reset_start();
    pframe = window.frame;
	RegionRenderer::static_init();
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
		rrenderers[i].use_region(&(world->regions[i]));
		rrenderers[i].init();
		rrenderers[i].prepare();
	}
	ShadowRenderer::static_init();
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
		srenderers[i].use_region(&(world->regions[i]));
		srenderers[i].use_world(*world);
		srenderers[i].init();
		srenderers[i].prepare();
	}

    fbuf.create(); fbtex.create(); fbrbuf.create();
    fbtex.bind(); fbtex.pixelate();
    fbtex.alloc_rgb(pframe.x, pframe.y);
    fbuf.attach_texture(fbtex, GL_COLOR_ATTACHMENT0);
    fbrbuf.bind();
    fbrbuf.alloc(GL_DEPTH24_STENCIL8, pframe.x, pframe.y);
    fbuf.attach_renderbuffer(fbrbuf, GL_DEPTH_STENCIL_ATTACHMENT);
    if (!fbuf.complete()) LOG_ERR("framebuffer failed!");
    fbuf.unbind();
    quad_shader = Shader::from_source("fullscreenv", "tex");
    quad = DefaultMeshes::tile<Vt_classic>();
    ol_shader = Shader::from_source("2Dmvp_vert", "color");
    outline = Mesh<vec2>::from_vectors({{0.,0.}, {0.,1.}, {1.,1.}, {1.,0.}},
                                        {0,1, 1,2, 2,3, 3,0});
    quad_perlin = Shader::from_source("fullscreenv", "perlin_bg_frag");
}

void WorldRenderer::prepare() {
    if (window.frame != pframe) {
        pframe = window.frame;
        fbtex.bind(); fbtex.pixelate();
        fbtex.alloc_rgb(pframe.x, pframe.y);
        fbuf.attach_texture(fbtex, GL_COLOR_ATTACHMENT0);
        fbrbuf.bind();
        fbrbuf.alloc(GL_DEPTH24_STENCIL8, pframe.x, pframe.y);
        fbuf.attach_renderbuffer(fbrbuf, GL_DEPTH_STENCIL_ATTACHMENT);
        if (!fbuf.complete()) LOG_ERR("framebuffer failed!");
        fbuf.unbind();
    }
}

void WorldRenderer::render() {

    // setup to render into post process buffer
    fbuf.bind(); gl.viewport(window.frame.x, window.frame.y);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // render shadow perlin bg
    glDisable(GL_STENCIL_TEST);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    quad_perlin.bind();
    quad_perlin.uVec2("uRes", window.frame);
    quad_perlin.uFloat("uTime", timer.read());
    quad_perlin.uFloat("uAspect", window.aspect);
    quad_perlin.uVec2("uCampos", cam->readPos().xy());
    quad.bind();
    gl.draw_mesh(quad);

    // turn off color mask, setup to increment stencil
    glEnable(GL_STENCIL_TEST);
    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    gl.wireframe(wf);

    // render shadow geometry
    ShadowRenderer::use_shader(ShadowRenderer::shadow_shader);
	ShadowRenderer::sync_camera(*cam);
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
		ivec2 const& rpos = world->regions[i].pos;
		if (abs((((float)REGION_SIZE*rpos.x)+(REGION_SIZE/2)) - (cam->readPos().x)) - REGION_SIZE/2 > (0.5*cam->readViewWidth())) continue;
		if (abs((((float)REGION_SIZE*rpos.y)+(REGION_SIZE/2)) - (cam->readPos().y)) - REGION_SIZE/2 > ((0.5*cam->readViewWidth()/window.aspect))) continue;
		srenderers[i].prepare();
	}
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
		ivec2 const& rpos = world->regions[i].pos;
		if (abs((((float)REGION_SIZE*rpos.x)+(REGION_SIZE/2)) - (cam->readPos().x)) - REGION_SIZE/2 > (0.5*cam->readViewWidth())) continue;
		if (abs((((float)REGION_SIZE*rpos.y)+(REGION_SIZE/2)) - (cam->readPos().y)) - REGION_SIZE/2 > ((0.5*cam->readViewWidth()/window.aspect))) continue;
        srenderers[i].prepare();
		srenderers[i].render();
	}

    // setup test to keep fragment if stencil == 0 (not in shadow)
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_EQUAL, 0, 0xFF);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // render terrain
	RegionRenderer::sync_camera(*cam);
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
		ivec2 const& rpos = world->regions[i].pos;
		if (abs((((float)REGION_SIZE*rpos.x)+(REGION_SIZE/2)) - (cam->readPos().x)) - REGION_SIZE/2 > (0.5*cam->readViewWidth())) continue;
		if (abs((((float)REGION_SIZE*rpos.y)+(REGION_SIZE/2)) - (cam->readPos().y)) - REGION_SIZE/2 > ((0.5*cam->readViewWidth()/window.aspect))) continue;
		rrenderers[i].prepare();
		rrenderers[i].render();
        world->regions[i].clear_flag();
	}

    // render entities TODO

    // render mouse hover tile outline
    vec2 mp = world->world_mpos(mpos, pframe, cam);
    ol_shader.bind();
    ol_shader.uMat4("uView", cam->view());
    ol_shader.uMat4("uProj", cam->proj());
    mat4 model = genModelMat2d((vec2)(world->pos_to_tpos(mp)), 0., vec2(1.));
    ol_shader.uMat4("uModel", model);
    gl.draw_mesh(outline, GL_LINES);
    // render mouse hover region outline
    model = genModelMat2d((vec2)(world->pos_to_rpos(mp) * REGION_SIZE), 0., vec2((float)REGION_SIZE));
    ol_shader.uMat4("uModel", model);
    outline.bind();
    gl.draw_mesh(outline, GL_LINES);

    // render post proc framebuffer to window framebuffer (no effects yet)
    glDisable(GL_STENCIL_TEST);
    Framebuffer::bind_default();
    glClear(GL_COLOR_BUFFER_BIT);
    quad.bind();
    fbtex.bind();
    quad_shader.bind();
    quad_shader.uInt("utex", 0);
    quad_shader.uFloat("uAspect", window.aspect);
    quad_shader.uVec2("uRes", window.frame);
    quad_shader.uFloat("uTime", timer.read());
    quad_shader.uVec2("uCampos", cam->readPos().xy());
    quad_shader.uFloat("uvw", cam->getViewWidth());
    static bool l = 0;
    if (window.keyboard[GLFW_KEY_L].pressed) l = !l;
    quad_shader.uFloat("ulightsw", l);
    quad.bind();
    gl.draw_mesh(quad);
}

void WorldRenderer::destroy() {
    RegionRenderer::static_destroy();
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
		rrenderers[i].destroy();
		srenderers[i].destroy();
	}
    fbuf.destroy(); fbtex.destroy(); fbrbuf.destroy();
    quad_shader.destroy(); quad.destroy();
    quad_perlin.destroy();
    outline.destroy();
    ol_shader.destroy();
}


