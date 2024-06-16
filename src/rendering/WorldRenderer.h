#ifndef WORLD_RENDERER_H
#define WORLD_RENDERER_H

#include <flgl.h>
#include <flgl/glm.h>
#include "RegionRenderer.h"
#include "ShadowRenderer.h"
#include "game/World.h"
#include <Stopwatch.h>

struct frame_manager_t {
    int w, h;
    /* new w&h based on cam, return true if w or h changed */
    bool update_wh(region_coords_t const& center, float camvw, float asp);

    struct frame_region_view_t {
        struct frame_region_iter;
        region_coords_t topleft, botright;
        void setup(region_coords_t const& center, int w, int h);

        frame_region_iter begin();
        frame_region_iter end();
        struct frame_region_iter {
            region_coords_t * tl, * br, pos;
            frame_region_iter(region_coords_t const& p, region_coords_t *b=0);
            bool operator==(frame_region_iter const& other) const;;;
            bool operator!=(frame_region_iter const& other) const;;
            frame_region_iter& operator++();
            region_coords_t operator*() const;
        };
    };

    frame_region_view_t region_viewer;
    frame_region_view_t regions_in_frame() const;
};



struct WorldRenderer : public Renderer {

    WorldRenderer() = default;
    virtual ~WorldRenderer() = default;

    void use_camera(OrthoCamera& c);
    void give_mouse(glm::ivec2 mp);
    void use_world(World& w);
    void twf();

    uint32_t get_pix_width() const;
    void set_pix_width(uint32_t pixels);
    void bump_pix_width(float bump);

    virtual void init() override final;
    virtual void prepare() override final;
    virtual void render() override final;
    virtual void destroy() override final;

private:
    Stopwatch timer;
    OrthoCamera* cam;
    World* world;
	RegionRenderer rrenderers[WORLD_DIAMETER*WORLD_DIAMETER];
	ShadowRenderer srenderers[WORLD_DIAMETER*WORLD_DIAMETER];

    Framebuffer fbuf;
    Texture fbtex;
    Renderbuffer fbrbuf;
    Shader quad_shader;
    Shader quad_perlin;
    Mesh<Vt_2Dclassic> quad;
    Mesh<glm::vec2> outline;
    Shader ol_shader;

    glm::ivec2 pframe;
    glm::ivec2 mpos;

    bool wf;
    float pixels;
    frame_manager_t frame_manager;
};




#endif
