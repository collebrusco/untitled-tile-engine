// 
// Components.h
// created 06/01/24 by frank collebrusco 
//
#ifndef COMPONENTS
#define COMPONENTS
#include <flgl/glm.h>
#include "../rendering/EntityRenderer.h"

struct c_Object {
    glm::vec2 pos, scale;
    float rot;
};

struct c_ERender {
    union {
        float data[4];
        glm::vec2 uvmin, uvmax;
    } uvs, poss;
    
};

#endif /* COMPONENTS */
