// #include "WorldRenderer.h"
// #include <flgl/logger.h>
// #include <flgl/glm.h>
// #include <flgl/tools.h>
// LOG_MODULE(wrend);
// using namespace glm;

// /* ============================== */
// /* ============================== */
// /* ============================== */
// /* ====   frame vao defns    ==== */
// /* ============================== */
// /* ============================== */
// /* ============================== */
// extern vec2 wcampos; extern float wcamvw; // TODO ...









// /* ============================== */
// /* ============================== */
// /* ============================== */
// /* ==== World Renderer Defns ==== */
// /* ============================== */
// /* ============================== */
// /* ============================== */

// WorldRenderer::WorldRenderer() : cam(&frame_manager) {}

// // void WorldRenderer::use_camera(OrthoCamera& c) {
// //     // cam = &c;
// // }

// void WorldRenderer::use_world(World& w) {
//     world = &w;
// }

// void WorldRenderer::give_mouse(glm::ivec2 mp) {
//     mpos = mp;
// }

// void WorldRenderer::twf() {wf = !wf;}

// // uint32_t WorldRenderer::get_pix_width() const {
// //     return (uint32_t)pixels;
// // }

// // void WorldRenderer::set_pix_width(uint32_t pix) {
// //     pixels = (float)pix;
// //     cam.setViewWidth((pixels) / 16.f);
// // }

// // void WorldRenderer::bump_pix_width(float bump) {
// //     pixels += bump;
// //     cam.setViewWidth(glm::floor(pixels) / 16.f);
// // }

// void WorldRenderer::init() {
//     pixels = 32*REGION_SIZE*2;
//     // set_pix_width(pixels);
//     timer.setUnit(SECONDS);
//     timer.reset_start();
//     pframe = window.frame;

//     // setup buffer of region renderers attached in pllel w world buffer
// 	RegionRenderer::static_init();
// 	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
// 		rrenderers[i].use_region(&(world->regions[i]));
// 		rrenderers[i].init();
// 		rrenderers[i].prepare();
// 	}

//     // setup buffer of shadow renderers attached in pllel w world buffer
//     ShadowRenderer::static_init();
// 	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
// 		srenderers[i].use_region(&(world->regions[i]));
// 		srenderers[i].use_world(*world);
// 		srenderers[i].init();
// 		srenderers[i].prepare();
// 	}

//     // setup framebuffer TODO replace
//     fbuf.create(); fbtex.create(); fbrbuf.create();
//     fbtex.bind(); fbtex.pixelate();
//     fbtex.alloc_rgb(pframe.x, pframe.y);
//     fbuf.attach_texture(fbtex, GL_COLOR_ATTACHMENT0);
//     fbrbuf.bind();
//     fbrbuf.alloc(GL_DEPTH24_STENCIL8, pframe.x, pframe.y);
//     fbuf.attach_renderbuffer(fbrbuf, GL_DEPTH_STENCIL_ATTACHMENT);
//     if (!fbuf.complete()) LOG_ERR("framebuffer failed!");
//     fbuf.unbind();

//     // frame_manager.update_wh(world->get_center(), wcamvw, window.aspect);
//     this->prepare();

//     quad_shader = Shader::from_source("fullscreenv", "tex");
//     quad_perlin = Shader::from_source("fullscreenv", "perlin_bg_frag");
//     ol_shader = Shader::from_source("2Dmvp_vert", "color");

//     quad = Mesh<Vt_2Dclassic>::from_vectors({{{-1.,-1.}, {0.,0.}},
// 									    	 {{-1., 1.}, {0.,1.}},
// 									    	 {{ 1., 1.}, {1.,1.}},
// 									    	 {{ 1.,-1.}, {1.,0.}}}
// 									    	 ,
// 									    	 {0, 2, 1,	0, 2, 3});
    
//     outline = Mesh<vec2>::from_vectors({{0.,0.}, {0.,1.}, {1.,1.}, {1.,0.}},
//                                         {0,1, 1,2, 2,3, 3,0});
//     frame_vao.create();
// }

// void WorldRenderer::prepare() {
//     // if (window.frame != pframe) {
//     //     pframe = window.frame;
//     //     fbtex.bind(); fbtex.pixelate();
//     //     fbtex.alloc_rgb(pframe.x, pframe.y);
//     //     fbuf.attach_texture(fbtex, GL_COLOR_ATTACHMENT0);
//     //     fbrbuf.bind();
//     //     fbrbuf.alloc(GL_DEPTH24_STENCIL8, pframe.x, pframe.y);
//     //     fbuf.attach_renderbuffer(fbrbuf, GL_DEPTH_STENCIL_ATTACHMENT);
//     //     if (!fbuf.complete()) LOG_ERR("framebuffer failed!");
//     //     fbuf.unbind();
//     // }
//     if (frame_manager.update_wh(world->get_center(), wcamvw, window.aspect)) {
//         ivec2 dim = frame_manager.get_fbuff_wh_pix();
//         LOG_INF("RESIZING TO %d,%d", dim.x, dim.y);
//         fbuf.destroy(); fbtex.destroy(); fbrbuf.destroy();
//         fbuf.create(); fbtex.create(); fbrbuf.create();
//         fbtex.bind(); fbtex.pixelate();
//         fbtex.alloc_rgb(dim.x, dim.y);
//         fbuf.attach_texture(fbtex, GL_COLOR_ATTACHMENT0);
//         fbrbuf.bind();
//         fbrbuf.alloc(GL_DEPTH24_STENCIL8, dim.x, dim.y);
//         fbuf.attach_renderbuffer(fbrbuf, GL_DEPTH_STENCIL_ATTACHMENT);
//         if (!fbuf.complete()) LOG_ERR("framebuffer failed!");
//         fbuf.unbind();
//         cam.getViewWidth() = frame_manager.get_fbuff_wh_tile().x;
//     }
//     vec3 cp = vec3(world->rpos_to_tpos(world->get_center()).x + REGION_SIZE/2,
//                 world->rpos_to_tpos(world->get_center()).y + REGION_SIZE/2, 0.f);
//     cam.getPos() = cp;
//     LOG_INF("wcam at %.0f,%.0f, moving cam to %.0f,%.0f", wcampos.x, wcampos.y, cp.x, cp.y);
// }

// void WorldRenderer::render() {

//     // setup to render into post process buffer
//     fbuf.bind(); 
//     {
//         ivec2 dim = frame_manager.get_fbuff_wh_pix();
//         gl.viewport(dim.x, dim.y);
//     }
//     glClear(GL_STENCIL_BUFFER_BIT | GL_COLOR_BUFFER_BIT);

//     // render shadow perlin bg
//     glDisable(GL_STENCIL_TEST);
//     glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);
//     quad_perlin.bind();
//     quad_perlin.uVec2("uRes", window.frame);
//     quad_perlin.uFloat("uTime", timer.read());
//     quad_perlin.uFloat("uAspect", window.aspect);
//     quad_perlin.uVec2("uCampos", cam.readPos().xy());
//     quad.bind();
//     gl.draw_mesh(quad);

//     // turn off color mask, setup to increment stencil
//     glEnable(GL_STENCIL_TEST);
//     glStencilFunc(GL_ALWAYS, 0, 0xFF);
//     glStencilOp(GL_KEEP, GL_KEEP, GL_INCR_WRAP);
//     glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);

//     gl.wireframe(wf);

//     // render shadow geometry
//     ShadowRenderer::use_shader(ShadowRenderer::shadow_shader);
// 	ShadowRenderer::sync_camera(cam);
//     for (auto pos : frame_manager.regions_in_frame()) {
//         size_t i = world->rpos_to_idx(pos);
// 		ivec2 const& rpos = world->regions[i].pos;
// 		srenderers[i].prepare();
// 	}
//     for (auto pos : frame_manager.regions_in_frame()) {
//         size_t i = world->rpos_to_idx(pos);
// 		ivec2 const& rpos = world->regions[i].pos;
//         srenderers[i].prepare();
// 		srenderers[i].render();
// 	}

//     // setup test to keep fragment if stencil == 0 (not in shadow)
//     glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
//     glStencilFunc(GL_EQUAL, 0, 0xFF);
//     glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);

//     // render terrain
// 	RegionRenderer::sync_camera(cam);
//     size_t ct = 0;
//     static size_t pct = 0;
//     for (auto pos : frame_manager.regions_in_frame()) {
//         ct++;
//         // LOG_INF("====ITER:==== %d, %d", pos.x, pos.y);
//         size_t i = world->rpos_to_idx(pos);
// 		ivec2 const& rpos = world->regions[i].pos;
// 		rrenderers[i].prepare();
// 		rrenderers[i].render();
//         world->regions[i].clear_flag();
// 	}
//     if (pct != ct) {pct = ct; LOG_INF("%d / %d renders", ct, WORLD_DIAMETER*WORLD_DIAMETER);}

//     // render entities TODO

//     // render mouse hover tile outline
//     vec2 mp = world->world_mpos(mpos, pframe, &cam);
//     ol_shader.bind();
//     ol_shader.uMat4("uView", cam.view());
//     ol_shader.uMat4("uProj", cam.proj());
//     mat4 model = genModelMat2d((vec2)(world->pos_to_tpos(mp)), 0., vec2(1.));
//     ol_shader.uMat4("uModel", model);
//     gl.draw_mesh(outline, GL_LINES);
//     // render mouse hover region outline
//     model = genModelMat2d((vec2)(world->pos_to_rpos(mp) * REGION_SIZE), 0., vec2((float)REGION_SIZE));
//     ol_shader.uMat4("uModel", model);
//     outline.bind();
//     gl.draw_mesh(outline, GL_LINES);

//     // render post proc framebuffer to window framebuffer (no effects yet)
//     glDisable(GL_STENCIL_TEST);
//     Framebuffer::bind_default();
//     glClear(GL_COLOR_BUFFER_BIT);
//     gl.viewport(window.frame.x, window.frame.y);
//     fbtex.bind();
//     quad_shader.bind();
//     quad_shader.uInt("utex", 0);
//     quad_shader.uFloat("uAspect", frame_manager.get_fbuff_aspect());
//     quad_shader.uVec2("uRes", frame_manager.get_fbuff_wh_pix());
//     quad_shader.uFloat("uTime", timer.read());
//     quad_shader.uVec2("uCampos", wcampos);
//     quad_shader.uFloat("uvw", wcamvw);
//     static bool l = 0;
//     if (window.keyboard[GLFW_KEY_L].pressed) l = !l;
//     quad_shader.uFloat("ulightsw", l);

//     VertexArray::unbind();
//     frame_vao.prepare(wcampos, wcamvw, window.aspect, world->get_center(), frame_manager.get_fbuff_wh_region());

//     frame_vao.vao.bind();
//     gl.draw_vao_ibo(frame_vao.vao, frame_vao.ibo);
//     // quad.bind();
//     // gl.draw_mesh(quad);
// }

// void WorldRenderer::destroy() {
//     RegionRenderer::static_destroy();
// 	for (int i = 0; i < WORLD_DIAMETER*WORLD_DIAMETER; i++) {
// 		rrenderers[i].destroy();
// 		srenderers[i].destroy();
// 	}
//     fbuf.destroy(); fbtex.destroy(); fbrbuf.destroy();
//     quad_shader.destroy(); quad.destroy();
//     quad_perlin.destroy();
//     outline.destroy();
//     ol_shader.destroy();
//     frame_vao.destroy();
// }


