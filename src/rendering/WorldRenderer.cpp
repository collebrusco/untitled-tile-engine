#include "WorldRenderer.h"
#include <flgl/logger.h>
#include <flgl/glm.h>
#include <flgl/tools.h>
LOG_MODULE(wrend);
using namespace glm;


bool frame_manager_t::update_wh(region_coords_t const& center, float camvw, float asp) {
    static region_coords_t pcenter{0xFFFFFFFF, 0xFFFFFFFF};
    int nx = ((int)ceil(camvw/(float)REGION_SIZE)+1)/2;
    int ny = ((int)ceil((camvw/(float)REGION_SIZE)/asp)+1)/2;

    bool needfbuff = false;
    if (nx != w || ny != h) {
        // LOG_INF("\t%d,%d rad\t\t%dx%d regions", nx, ny, (nx*2)+1, (ny*2)+1);
        needfbuff = true;
    }
    if (needfbuff || pcenter != center) {
        region_viewer.setup(center, nx, ny);
    }
    w = nx; h = ny;
    return needfbuff;
}

ivec2 frame_manager_t::get_fbuff_wh_pix() const {
    return ivec2(
        min((region_viewer.botright.x - region_viewer.topleft.x + 1), WORLD_DIAMETER) * REGION_SIZE * 32,
        min((region_viewer.topleft.y - region_viewer.botright.y + 1), WORLD_DIAMETER) * REGION_SIZE * 32
    );
}

glm::ivec2 frame_manager_t::get_fbuff_wh_tile() const {
    return ivec2(
        min((region_viewer.botright.x - region_viewer.topleft.x + 1), WORLD_DIAMETER) * REGION_SIZE,
        min((region_viewer.topleft.y - region_viewer.botright.y + 1), WORLD_DIAMETER) * REGION_SIZE
    );
}

glm::ivec2 frame_manager_t::get_fbuff_wh_region() const {
    return ivec2(
        region_viewer.botright.x - region_viewer.topleft.x + 1,
        region_viewer.topleft.y - region_viewer.botright.y + 1
    );
}
    
void frame_manager_t::frame_region_view_t::setup(region_coords_t const& center, int w, int h) {
    topleft  = center + ivec2(-(min(WORLD_DIAMETER/2, w)), min((WORLD_DIAMETER/2)-1, h));
    botright = center + ivec2(min((WORLD_DIAMETER/2)-1, w), -(min(WORLD_DIAMETER/2, h)));
    // topleft  = center + glm::ivec2(-w, h);
    // botright = center + glm::ivec2(w, -h);
    // LOG_INF("expect %d,%d, getting %d,%d", min(2*w+1, WORLD_DIAMETER), min(2*h+1, WORLD_DIAMETER), botright.x - topleft.x + 1, topleft.y - botright.y + 1);
}

frame_manager_t::frame_region_view_t::frame_region_iter frame_manager_t::frame_region_view_t::begin() {
    frame_region_iter it(topleft);
    it.tl = &topleft; it.br = &botright;
    return it;
}

frame_manager_t::frame_region_view_t::frame_region_iter frame_manager_t::frame_region_view_t::end() {
    return frame_region_iter(glm::ivec2(botright.x+1, botright.y), 0);
}

frame_manager_t::frame_region_view_t::frame_region_iter::
frame_region_iter(region_coords_t const& p, region_coords_t *b) 
: pos(p), br(b) {}

bool frame_manager_t::frame_region_view_t::frame_region_iter::operator==(frame_region_iter const& other) const {
    return (!this->br && !other.br) || (this->pos == other.pos);
}
bool frame_manager_t::frame_region_view_t::frame_region_iter::operator!=(frame_region_iter const& other) const {
    return !this->operator==(other);
}

frame_manager_t::frame_region_view_t::frame_region_iter& frame_manager_t::frame_region_view_t::frame_region_iter::operator++() {
    if (!br) return *this;
    if (*br == pos) {
        br = 0;
        return *this;
    }
    pos.x++;
    if (pos.x > br->x) {
        pos.x = tl->x;
        pos.y--;
    }
    return *this;
}

region_coords_t frame_manager_t::frame_region_view_t::frame_region_iter::operator*() const {
    return pos;
}

frame_manager_t::frame_region_view_t frame_manager_t::regions_in_frame() const {
    return region_viewer;
}

/* ==== World Renderer Defns ==== */

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

uint32_t WorldRenderer::get_pix_width() const {
    return (uint32_t)pixels;
}

void WorldRenderer::set_pix_width(uint32_t pix) {
    pixels = (float)pix;
    cam->setViewWidth((pixels) / 16.f);
}

void WorldRenderer::bump_pix_width(float bump) {
    pixels += bump;
    cam->setViewWidth(glm::floor(pixels) / 16.f);
}

void WorldRenderer::init() {
    pixels = 32*REGION_SIZE*2;
    set_pix_width(pixels);
    timer.setUnit(SECONDS);
    timer.reset_start();
    pframe = window.frame;

    // setup buffer of region renderers attached in pllel w world buffer
	RegionRenderer::static_init();
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
		rrenderers[i].use_region(&(world->regions[i]));
		rrenderers[i].init();
		rrenderers[i].prepare();
	}

    // setup buffer of shadow renderers attached in pllel w world buffer
    ShadowRenderer::static_init();
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
		srenderers[i].use_region(&(world->regions[i]));
		srenderers[i].use_world(*world);
		srenderers[i].init();
		srenderers[i].prepare();
	}

    // setup framebuffer TODO replace
    fbuf.create(); fbtex.create(); fbrbuf.create();
    fbtex.bind(); fbtex.pixelate();
    fbtex.alloc_rgb(pframe.x, pframe.y);
    fbuf.attach_texture(fbtex, GL_COLOR_ATTACHMENT0);
    fbrbuf.bind();
    fbrbuf.alloc(GL_DEPTH24_STENCIL8, pframe.x, pframe.y);
    fbuf.attach_renderbuffer(fbrbuf, GL_DEPTH_STENCIL_ATTACHMENT);
    if (!fbuf.complete()) LOG_ERR("framebuffer failed!");
    fbuf.unbind();

    frame_manager.update_wh(world->get_center(), cam->getViewWidth(), window.aspect);

    quad_shader = Shader::from_source("fullscreenv", "tex");
    quad_perlin = Shader::from_source("fullscreenv", "perlin_bg_frag");
    ol_shader = Shader::from_source("2Dmvp_vert", "color");

    quad = Mesh<Vt_2Dclassic>::from_vectors({{{-1.,-1.}, {0.,0.}},
									    	 {{-1., 1.}, {0.,1.}},
									    	 {{ 1., 1.}, {1.,1.}},
									    	 {{ 1.,-1.}, {1.,0.}}}
									    	 ,
									    	 {0, 2, 1,	0, 2, 3});
    
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
    if (frame_manager.update_wh(world->get_center(), cam->readViewWidth(), window.aspect)) {

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
    for (auto pos : frame_manager.regions_in_frame()) {
        size_t i = world->rpos_to_idx(pos);
		ivec2 const& rpos = world->regions[i].pos;
		srenderers[i].prepare();
	}
    for (auto pos : frame_manager.regions_in_frame()) {
        size_t i = world->rpos_to_idx(pos);
		ivec2 const& rpos = world->regions[i].pos;
        srenderers[i].prepare();
		srenderers[i].render();
	}

    // setup test to keep fragment if stencil == 0 (not in shadow)
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_EQUAL, 0, 0xFF);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // render terrain
	RegionRenderer::sync_camera(*cam);
    size_t ct = 0;
    static size_t pct = 0;
    for (auto pos : frame_manager.regions_in_frame()) {
        ct++;
        // LOG_INF("====ITER:==== %d, %d", pos.x, pos.y);
        size_t i = world->rpos_to_idx(pos);
		ivec2 const& rpos = world->regions[i].pos;
		rrenderers[i].prepare();
		rrenderers[i].render();
        world->regions[i].clear_flag();
	}
    if (pct != ct) {pct = ct; LOG_INF("%d / %d renders", ct, WORLD_DIAMETER*WORLD_DIAMETER);}

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


