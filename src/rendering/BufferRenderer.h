// 
// BufferRenderer.h
// created 06/16/24 by frank collebrusco 
//
#ifndef BUFFER_RENDERER
#define BUFFER_RENDERER

#include <flgl.h>
#include <flgl/glm.h>
#include "RegionRenderer.h"
#include "ShadowRenderer.h"
#include "EntityRenderer.h"
#include "data/World.h"
#include <Stopwatch.h>
#include "frame_manager.h"
#include "NWOcam.h"
#include "data/render_pipeline_structs.h"

struct BufferRenderer {

    BufferRenderer();
    virtual ~BufferRenderer() = default;

    inline buffrender_details_t* input_ptr() {return &input;}
    void write_output(postrender_details_t * output);

    void init();
    void prepare();
    void render();
    void destroy();

    NWOcam cam;
    frame_manager_t frame_manager;
private:
    Stopwatch timer;
    
    buffrender_details_t input;

	RegionRenderer rrenderers[WORLD_DIAMETER*WORLD_DIAMETER];
	ShadowRenderer srenderers[WORLD_DIAMETER*WORLD_DIAMETER];
    EntityRenderer erenderer;
    
	Texture tile_tex;

    Framebuffer fbuf;
    Texture fbtex;
    Renderbuffer fbrbuf;
    Shader quad_shader;
    Shader quad_perlin;
    Shader ol_shader;
    Mesh<Vt_2Dclassic> quad;
    Mesh<glm::vec2> outline;

    bool wf;
};

#endif /* BUFFER_RENDERER */
