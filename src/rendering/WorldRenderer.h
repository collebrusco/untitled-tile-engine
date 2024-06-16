#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include <flgl.h>
#include <flgl/glm.h>
#include "RegionRenderer.h"
#include "ShadowRenderer.h"
#include "game/World.h"
#include <Stopwatch.h>
#include "frame_manager.h"
#include "NWOcam.h"

struct frame_vao_t {
    VertexArray vao;
    VertexBuffer<glm::vec2> posbuf;
    VertexBuffer<glm::vec2> uvbuf;
    ElementBuffer ibo;
    void create();
    void destroy();
    void prepare(glm::vec2 campos, float vw, float asp, glm::ivec2 center, glm::ivec2 framewh);
};

struct WorldRenderer : public Renderer {

    WorldRenderer();
    virtual ~WorldRenderer() = default;

    // void use_camera(OrthoCamera& c);
    void give_mouse(glm::ivec2 mp);
    void use_world(World& w);
    void twf();

    // uint32_t get_pix_width() const;
    // void set_pix_width(uint32_t pixels);
    // void bump_pix_width(float bump);

    virtual void init() override final;
    virtual void prepare() override final;
    virtual void render() override final;
    virtual void destroy() override final;

    NWOcam cam;
private:
    Stopwatch timer;
    World* world;
	RegionRenderer rrenderers[WORLD_DIAMETER*WORLD_DIAMETER];
	ShadowRenderer srenderers[WORLD_DIAMETER*WORLD_DIAMETER];

    Framebuffer fbuf;
    Texture fbtex;
    Renderbuffer fbrbuf;
    Shader quad_shader;
    Shader quad_perlin;
    Shader ol_shader;
    Mesh<Vt_2Dclassic> quad;
    Mesh<glm::vec2> outline;
    frame_vao_t frame_vao;

    glm::ivec2 pframe;
    glm::ivec2 mpos;

    bool wf;
    float pixels;
    frame_manager_t frame_manager;
};




#endif
