// 
// frame_manager.h
// created 06/16/24 by frank collebrusco 
//
#ifndef FRAME_MANAGER
#define FRAME_MANAGER
#include <flgl/glm.h>
#include "game/Region.h"

struct frame_manager_t {
    int w, h;
    /* new w&h based on cam, return true if w or h changed */
    bool update_wh(region_coords_t const& center, float camvw, float asp);
    glm::ivec2 get_fbuff_wh_pix() const;
    glm::ivec2 get_fbuff_wh_tile() const;
    glm::ivec2 get_fbuff_wh_region() const;
    float get_fbuff_aspect() const;

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



#endif /* FRAME_MANAGER */
