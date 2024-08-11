/** 
 * Events.h
 * created 07/08/24 by frank collebrusco 
 */
#ifndef EVENTS_H
#define EVENTS_H
#include "Event.h"

EVENT_DECLARE(ActorMove) {
    entID actor;
    glm::vec2 oldpos, newpos;
};

#endif /* EVENTS_H */
