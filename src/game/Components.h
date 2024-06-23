// 
// Components.h
// created 06/01/24 by frank collebrusco 
//
#ifndef COMPONENTS
#define COMPONENTS
#include <flgl/glm.h>


struct c_Object {
    glm::vec2 pos, scale;
    float rot;
};

struct c_StaticRender {
    glm::vec2 pos;
};

#endif /* COMPONENTS */
