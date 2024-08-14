/** 
 * Movement.h
 * created 08/03/24 by frank collebrusco 
 */
#ifndef MOVEMENT_H
#define MOVEMENT_H
#include "ECS.h"
#include "data/components.h"
#include "data/World.h"

#define CMOVE_FRICTION_FULL 0x7F

/**
 * c_Move represents a move w velocity v in tiles/sec
 * clip_rad is bounding radius
 */
struct __attribute__((packed)) c_Move {
    glm::vec2 v;
    float clip_rad;
    struct {
        bool clip           : 1;
        uint8_t friction    : 7;
    } props;
    static void execute_moves(float dt, World* const world);
};

struct a_Movable : public Archetype<c_Move, c_Object> {};

#endif /* MOVEMENT_H */
