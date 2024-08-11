/** 
 * Movement.h
 * created 08/03/24 by frank collebrusco 
 */
#ifndef MOVEMENT_H
#define MOVEMENT_H
#include "ECS.h"
#include "game/components.h"
#include "game/World.h"

/**
 * c_Move represents a move w velocity v in tiles/sec
 * clip_rad is bounding radius, set negative for no tile collisions
 */
struct __attribute__((packed)) c_Move {
    glm::vec2 v;
    float clip_rad; /* if < 0, no clip */
    static void execute_moves(float dt, World* const world);
};

struct a_Movable : public Archetype<c_Move, c_Object> {};

#endif /* MOVEMENT_H */