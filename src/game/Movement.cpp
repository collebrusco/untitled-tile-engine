#include "Movement.h"
#include <flgl/logger.h>
#include <stdlib.h>
LOG_MODULE("mvmnt");
using namespace glm;

static bool inline point_in_rad(vec2 pt, vec2 c, float r) {
    return length(pt - c) < r;
}

/* OPT improve this? */
static bool tile_on_edge(tile_coords_t tp, vec2 c, float r) {
    uint8_t n = 0;
    n += (uint8_t)point_in_rad((vec2)tp,               c, r);
    n += (uint8_t)point_in_rad((vec2){tp.x  , tp.y+1}, c, r);
    n += (uint8_t)point_in_rad((vec2){tp.x+1, tp.y  }, c, r);
    n += (uint8_t)point_in_rad((vec2){tp.x+1, tp.y+1}, c, r);
    /* at least one but not all corners within rad means yes */
    if (n && n < 4) return true;
    /* special case: circle butts into tile edge but no corners */
    if (World::pos_to_tpos(vec2{((float)tp.x < c.x) ? c.x - r : c.x + r, c.y}) == tp) {
        return true;
    }
    if (World::pos_to_tpos(vec2{c.x, ((float)tp.y < c.y) ? c.y - r : c.y + r}) == tp) {
        return true;
    }
    return false;
}

static bool tile_col_check(World* const world, vec2 c, float r) {
    ivec2 mi = World::pos_to_tpos({c.x - r, c.y - r});
    ivec2 ma = World::pos_to_tpos({c.x + r, c.y + r});
    if (mi == ma) return false; /* rad stays in center tile, no col */
    for (int x = mi.x; x <= ma.x; x++) {
        for (int y = mi.y; y <= ma.y; y++) {
            if (tile_on_edge({x,y}, c, r)) {
                Tile const& t = world->read_tile_at((tile_coords_t){x,y});
                if (t.surf.props.f.present && t.surf.props.f.solid) {
                    return true;
                }
            }
        }
    }
    return false;
}

void c_Move::execute_moves(float dt, World* const world) {
    for (entID e : world->view((a_Movable*)0)) {
        auto& obj = world->getComp<c_Object>(e);
        auto& mov = world->getComp<c_Move>(e);
        auto const pos = obj.pos;
        auto vec = mov.v * dt;
        /* no clip case */
        if (mov.clip_rad < 0.f) {
            obj.pos += vec;
            return;
        }
        /* step */
#if 1
        auto pa = World::pos_to_tpos(pos);
        auto pb = World::pos_to_tpos(pos + vec);
        {
            if (abs(pa.x - pb.x) > 1 ||
                abs(pa.y - pb.y) > 1) {
                    LOG_ERR("move over > 1 tile in one frame, currently unsupported. Add stepper to support that speed");
                }
        }
#endif

        /* edit move to be legal (deflect, block) */
        if (tile_col_check(world, pos + vec, mov.clip_rad)) {
            /* collision in (x + y) */
            if (tile_col_check(world, {pos.x + vec.x, pos.y}, mov.clip_rad)) {
                /* collision in (x+y), x */
                if (tile_col_check(world, {pos.x, pos.y + vec.y}, mov.clip_rad)) {
                    /* collides in (x+y), x, & y, no moving */
                    vec = {0.f, 0.f};
                } else {
                    /* collision in (x+y) & x only, allow y */
                    vec.x = 0.f;
                }
            } else {
                /* no collision in x alone */
                if (tile_col_check(world, {pos.x, pos.y + vec.y}, mov.clip_rad)) {
                    /* collision in y only, allow x */
                    vec.y = 0.f;
                } else {
                    /* this would mean col in (x+y), not x or y alone. pick random */
                    if (rand()&0x01u)
                        vec.x = 0.f;
                    else
                        vec.y = 0.f;
                }
            }
        }

        /* complete move */
        obj.pos += vec;
        world->removeComp<c_Move>(e);
    }
}

