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

enum rotation_type_e {
    ROTATE_NONE = 0,
    ROTATE_FULL,
    ROTATE_2_PT,
    ROTATE_4_PT,
    ROTATE_6_PT,
    ROTATE_VAXIS_MIRROR,
    ROTATE_HAXIS_MIRROR
};

struct __attribute__((packed)) ent_render_props_t {
    rotation_type_e rot : 3;
    uint8_t pix_snap    : 1;
};

struct c_EntRenderEntry {
    struct {
        glm::vec2 bl, tr;
    } uvs;

    ent_render_props_t props;
    
    /** 
     * @param sheetblpos bot-left corner of sprite  (tile unit coords)
     * @param size       size of sprite             (tile unit coords)
     */
    static inline constexpr c_EntRenderEntry from_sheet(glm::vec2 sheetblpos, glm::vec2 size, rotation_type_e rt, uint8_t snap) {
        sheetblpos.y = TILE_SPRITESHEET_DIM_F - sheetblpos.y; size.y *= -1.f;
        return (c_EntRenderEntry) {
            .uvs = {
                .bl = sheetblpos / (TILE_SPRITESHEET_DIM_F),
                .tr = (sheetblpos + size) / (TILE_SPRITESHEET_DIM_F)
            },
            .props = {
                .rot = rt,
                .pix_snap = snap
            }
        };
    }

};


#endif /* COMPONENTS */
