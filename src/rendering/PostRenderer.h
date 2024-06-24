// 
// PostRenderer.h
// created 06/16/24 by frank collebrusco 
//
#ifndef POST_RENDERER
#define POST_RENDERER

#include <flgl.h>
#include <flgl/glm.h>
#include "Stopwatch.h"
#include "Renderer.h"
#include "NWOcam.h"
#include "render_pipeline_structs.h"

struct frame_vao_t {
    VertexArray vao;
    VertexBuffer<glm::vec2> posbuf;
    VertexBuffer<glm::vec2> uvbuf;
    ElementBuffer ibo;
    void create();
    void destroy();
    void prepare(glm::vec2 campos, glm::vec2 lframe, glm::ivec2 blpos, glm::ivec2 framewh);
};

struct PostRenderer {

    PostRenderer();
    virtual ~PostRenderer() = default;

    void twf();

    inline postrender_details_t* input_ptr() {return &details;}

    void init();
    void prepare();
    void render();
    void destroy();

private:
    Stopwatch timer;
    postrender_details_t details;

    Shader post_shader;

    frame_vao_t frame_vao;
    Mesh<Vt_classic> testq;

    glm::ivec2 pframe;

    bool wf;
};


#endif /* POST_RENDERER */
