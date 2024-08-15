/** 
 * NewAnimation.h
 * created 08/15/24 by frank collebrusco 
 */
#ifndef NEW_ANIMATION_H
#define NEW_ANIMATION_H
#include "ECS.h"
#include <flgl.h>
#include <flgl/glm.h>

struct MeshLayer {
    virtual void add_es(Vt_classic* verts, uint32_t* vtop, uint32_t* elems, uint32_t* etop) const = 0;
};

struct Legs : public MeshLayer {
    virtual void add_es(Vt_classic* verts, uint32_t* vtop, uint32_t* elems, uint32_t* etop) const override final;
    // void use_sprite(Sprite const*const sp);
};

#endif /* NEW_ANIMATION_H */
