#include "BufferRenderer.h"
#include <flgl/logger.h>
LOG_MODULE(bufrend);
using namespace glm;

BufferRenderer::BufferRenderer() : cam(&frame_manager) {}

void BufferRenderer::init() {
    // set_pix_width(pixels);
    timer.setUnit(SECONDS);
    timer.reset_start();

    // setup buffer of region renderers attached in pllel w world buffer
	RegionRenderer::static_init();
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
        if (!input.world) {LOG_ERR("BAD WORLD");}
		rrenderers[i].use_region(&(input.world->regions[i]));
		rrenderers[i].init();
		rrenderers[i].prepare();
	}

    // setup buffer of shadow renderers attached in pllel w world buffer
    ShadowRenderer::static_init();
	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
		srenderers[i].use_region(&(input.world->regions[i]));
		srenderers[i].use_world(*(input.world));
		srenderers[i].init();
		srenderers[i].prepare();
	}

    // setup framebuffer TODO 

    // frame_manager.update_wh(input.world->get_center(), input.lcam.frame.x, window.aspect);
    this->prepare();

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

void BufferRenderer::prepare() {
    static region_coords_t pcenter{0xFFFFFFFF,0xFFFFFFFF};
    region_coords_t center = input.world->get_center();
    if (pcenter != center) {
        vec3 np = vec3(
            (float)(input.world->rpos_to_tpos(center).x + (REGION_SIZE/2)) + 0.5f,
            (float)(input.world->rpos_to_tpos(center).y + (REGION_SIZE/2)) + 0.5f,
            1.f
        );
        cam.getPos() = np;
        LOG_INF("buf cam pos to %.0f,%.0f", cam.readPos().x, cam.readPos().y);
    }
    pcenter = center;

    bool new_size = frame_manager.update_wh(input.world->get_center(), input.lcam.frame.x, window.aspect);
    if (new_size) {
        ivec2 dim = frame_manager.get_fbuff_wh_pix();
        // LOG_INF("RESIZING TO %d,%d", dim.x, dim.y);

        fbuf.destroy(); fbtex.destroy(); fbrbuf.destroy();
        fbuf.create(); fbtex.create(); fbrbuf.create();
        fbtex.bind(); fbtex.pixelate();
        fbtex.alloc_rgb(dim.x, dim.y);
        fbuf.bind();
        fbuf.attach_texture(fbtex, GL_COLOR_ATTACHMENT0);
        fbrbuf.bind();
        fbrbuf.alloc(GL_DEPTH24_STENCIL8, dim.x, dim.y);
        fbuf.attach_renderbuffer(fbrbuf, GL_DEPTH_STENCIL_ATTACHMENT);
        if (!fbuf.complete()) LOG_ERR("framebuffer failed!");
        fbuf.unbind(); fbtex.unbind(); fbrbuf.unbind();

        cam.getViewWidth() = frame_manager.get_fbuff_wh_tile().x;
    }

    cam.update();
}

void BufferRenderer::render() {

    // setup to render into post process buffer
    fbuf.bind(); 
    {
        ivec2 dim = frame_manager.get_fbuff_wh_pix();
        gl.viewport(dim.x, dim.y);
    }
    glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

    // render shadow perlin bg
    glDisable(GL_STENCIL_TEST);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
    quad_perlin.bind();
    quad_perlin.uVec2("uRes", frame_manager.get_fbuff_wh_pix());
    quad_perlin.uFloat("uTime", timer.read());
    quad_perlin.uFloat("uAspect", frame_manager.get_fbuff_aspect());
    quad_perlin.uVec2("uCampos", input.lcam.pos);
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
	ShadowRenderer::sync_camera(cam);
    for (auto pos : frame_manager.regions_in_frame()) {
        size_t i = input.world->rpos_to_idx(pos);
		ivec2 const& rpos = input.world->regions[i].pos;
		srenderers[i].prepare();
	}
    for (auto pos : frame_manager.regions_in_frame()) {
        size_t i = input.world->rpos_to_idx(pos);
		ivec2 const& rpos = input.world->regions[i].pos;
        srenderers[i].prepare();
		srenderers[i].render();
	}

    // setup test to keep fragment if stencil == 0 (not in shadow)
    glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
    glStencilFunc(GL_EQUAL, 0, 0xFF);
    glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

    // render terrain
	RegionRenderer::sync_camera(cam);
    size_t ct = 0;
    static size_t pct = 0;
    for (auto pos : frame_manager.regions_in_frame()) {
        ct++;
        // LOG_INF("====ITER:==== %d, %d", pos.x, pos.y);
        size_t i = input.world->rpos_to_idx(pos);
		// ivec2 const& rpos = input.world->regions[i].pos;
		rrenderers[i].prepare();
		rrenderers[i].render();
        input.world->regions[i].clear_flag();
	}
    if (pct != ct) {pct = ct; LOG_INF("%d / %d renders", ct, WORLD_DIAMETER*WORLD_DIAMETER);}

    // render entities TODO

    // render mouse hover tile outline
    vec2 mp = input.world->world_mpos(window.mouse.pos, window.frame, &cam); // TODO GET WORLD
    ol_shader.bind();
    ol_shader.uMat4("uView", cam.view());
    ol_shader.uMat4("uProj", cam.proj());
    mat4 model = genModelMat2d((vec2)(input.world->pos_to_tpos(mp)), 0., vec2(1.));
    ol_shader.uMat4("uModel", model);
    gl.draw_mesh(outline, GL_LINES);
    // render mouse hover region outline
    model = genModelMat2d((vec2)(input.world->pos_to_rpos(mp) * REGION_SIZE), 0., vec2((float)REGION_SIZE));
    ol_shader.uMat4("uModel", model);
    outline.bind();
    gl.draw_mesh(outline, GL_LINES);

    Framebuffer::bind_default(); VertexArray::unbind();
    glDisable(GL_STENCIL_TEST);
    // write to output
}

void BufferRenderer::write_output(postrender_details_t * output) {
    ivec2 blpos = ivec2(frame_manager.region_viewer.topleft.x, frame_manager.region_viewer.botright.y);
    (*output) = {
        .fbuf = this->fbuf,
        .fbtex = this->fbtex,
        .frame_pix = frame_manager.get_fbuff_wh_pix(),
        .world_blpos = blpos,
        .local_cam = this->input.lcam
    };
}


void BufferRenderer::destroy() {
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


