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

    fbuf.bind(); gl.viewport(window.frame.x, window.frame.y);
    
    glEnable(GL_STENCIL_TEST);
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    glStencilFunc(GL_ALWAYS, 0, 0xFF);
    glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP);
    glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

    gl.wireframe(wf);

    ShadowRenderer::use_shader(ShadowRenderer::shadow_shader);
	ShadowRenderer::sync_camera(*cam);
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
		ivec2 const& rpos = world->regions[i].pos;
		if (abs((((float)REGION_SIZE*rpos.x)+(REGION_SIZE/2)) - (cam->readPos().x)) - REGION_SIZE/2 > (0.5*cam->readViewWidth())) continue;
		if (abs((((float)REGION_SIZE*rpos.y)+(REGION_SIZE/2)) - (cam->readPos().y)) - REGION_SIZE/2 > ((0.5*cam->readViewWidth()/window.aspect))) continue;
		srenderers[i].prepare();
		srenderers[i].render();
	}

    glStencilOp(GL_KEEP, GL_KEEP, GL_DECR);
    gl.wireframe(0);

    ShadowRenderer::use_shader(ShadowRenderer::base_shader);
	ShadowRenderer::sync_camera(*cam);
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
		ivec2 const& rpos = world->regions[i].pos;
		if (abs((((float)REGION_SIZE*rpos.x)+(REGION_SIZE/2)) - (cam->readPos().x)) - REGION_SIZE/2 > (0.5*cam->readViewWidth())) continue;
		if (abs((((float)REGION_SIZE*rpos.y)+(REGION_SIZE/2)) - (cam->readPos().y)) - REGION_SIZE/2 > ((0.5*cam->readViewWidth()/window.aspect))) continue;
		srenderers[i].prepare();
		srenderers[i].render();
	}

    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_EQUAL, 0, 0xFF);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

	RegionRenderer::sync_camera(*cam);
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
		ivec2 const& rpos = world->regions[i].pos;
		if (abs((((float)REGION_SIZE*rpos.x)+(REGION_SIZE/2)) - (cam->readPos().x)) - REGION_SIZE/2 > (0.5*cam->readViewWidth())) continue;
		if (abs((((float)REGION_SIZE*rpos.y)+(REGION_SIZE/2)) - (cam->readPos().y)) - REGION_SIZE/2 > ((0.5*cam->readViewWidth()/window.aspect))) continue;
		rrenderers[i].prepare();
		rrenderers[i].render();
        world->regions[i].clear_flag();
	}

    vec2 mp = world->world_mpos(mpos, pframe, cam);
    ol_shader.bind();
    ol_shader.uMat4("uView", cam->view());
    ol_shader.uMat4("uProj", cam->proj());
    mat4 model = genModelMat2d((vec2)(world->pos_to_tpos(mp)), 0., vec2(1.));
    ol_shader.uMat4("uModel", model);
    gl.draw_mesh(outline, GL_LINES);
    model = genModelMat2d((vec2)(world->pos_to_rpos(mp) * REGION_SIZE), 0., vec2((float)REGION_SIZE));
    ol_shader.uMat4("uModel", model);
    outline.bind();
    gl.draw_mesh(outline, GL_LINES);

    Framebuffer::bind_default();
    glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
    quad.bind();
    fbtex.bind();
    quad_shader.bind();
    quad_shader.uInt("utex", 0);
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
    outline.destroy();
    ol_shader.destroy();
}


