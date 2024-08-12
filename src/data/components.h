// 
// Components.h
// created 06/01/24 by frank collebrusco 
//
#ifndef COMPONENTS
#define COMPONENTS
#include <stdint.h>
#include "constants.h"
#include <flgl/glm.h>

struct c_Object {
    glm::vec2 pos, scale;
    float rot;
    glm::vec2 anc;
};

struct c_StaticAtlas {
    struct {
        glm::vec2 bl, tr;
    } uvs;

    inline c_StaticAtlas copy() const {
        c_StaticAtlas c; c.uvs.bl = this->uvs.bl; c.uvs.tr = this->uvs.tr; return c;
    }
    
    /** 
     * @param sheetblpos bot-left corner of sprite  (tile unit coords)
     * @param size       size of sprite             (tile unit coords)
     */
    static inline constexpr c_StaticAtlas from_sheet(glm::vec2 sheetblpos, glm::vec2 size) {
        sheetblpos.y = TILE_SPRITESHEET_DIM_F - sheetblpos.y; size.y *= -1.f;
        return (c_StaticAtlas) {
            .uvs = {
                .bl = sheetblpos / (TILE_SPRITESHEET_DIM_F),
                .tr = (sheetblpos + size) / (TILE_SPRITESHEET_DIM_F)
            }
        };
    }

};


#endif /* COMPONENTS */
