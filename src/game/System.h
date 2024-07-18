/** 
 * System.h
 * created 06/22/24 by frank collebrusco 
 */
#ifndef SYSTEM_H
#define SYSTEM_H
#include <flgl.h>
#include "game/State.h"

/**
 * ooh what if each sys has an event queue
 * rather any sub it would have to be.
 * so when events fire they just enqueue in their subs
 * then we control the order of the updates by ordering their syncs
 * 
 * thats just another idea maybe make it optional immediate, sub queue, global queue 3 ideas
 * 
 * i can see using this
 * camera system
 *  has lcam
 *  has update (fix asp)
 *  has getters & lcam stuff
 *  subs to player move?
 *  ug but what if diff sys's are ref'ing lcam pos before and after? man
 *  
 * day 2
 * if we separate all code that fires events into sys's and stuff that run first and then event queue flush
 * and events or actions or whatever all have copies of the old data they need to exec
 * 
 * then we have a system i think. we'd have 
 * 
 *          |
 *      new frame:
 *      new frame state
 *          |
 *      systems, actors, etc all exec, 
 *      pushing events or actions to queues 
 *      containing snapshots of state they need
 *          |
 *      event, action and system queues
 *      wait.. events and actions are the same thing. tangent
 *      anyway all of the queues carry out
 *          |
 *      next frame state
 *          |
 *      render and stuff
 * 
 * 
 * 
 * 
 *      so events and actions are the same thing. 
 *      instead of an overriden action subtype,
 *      we've got a global event instance temp'd with input data
 *      the template spec is analogous to subtyping w data needed to carry out action
 *      except here each action type holds its own queue of instances of itself that need to go off
 *      they now each have subscriber lists as well so that anything can sub to the action occuring & react
 * 
 *      ok so this seems like a good idea to do this we need special action events maybe that extend and only diff is they have 
 *      well 
 * 
 * 
 *      no wait events are not actions because actions are more detailed perhaps iterative processes that occur in the world
 *      the diff being that actions need 
 *          1. some sort of action.fire(dt); if (action.complete()) pop(action); 
 *          2. the ability to fire off events as they go. 
 * 
 *      they're, higher level? events being a central bus for various systems to react to
 *      and actions don't need subs because they can simply define and fire events for that
 * 
 *      but actions i wanna think of as in game world processes that things can do.
 *      you need to throw a knife? spawn knife ent, step its movement, detect collision, delete?
 *      
 *       
 * 
 */

typedef void(*system_init_f)(State* state);
typedef void(*system_dest_f)(State* state);
typedef void(*system_step_f)(float dt, State* state, Keyboard const& kb, Mouse const& mouse, glm::vec2 wmpos, glm::vec2 wmdelt);

struct system_t {
    system_init_f init;
    system_step_f step;
    system_dest_f dest;
    float period_s;
    float last_time_s;
};

#define MAX_SYSTEMS_LOG (4)
#define MAX_SYSTEMS (1<<MAX_SYSTEMS_LOG)


class SystemList {
public:
    SystemList();
    system_t systems[MAX_SYSTEMS];
    system_t& next_system();
private:
    size_t top  : MAX_SYSTEMS_LOG;
    class SystemIt {
        friend class SystemList;
        SystemIt(SystemList& l, bool d = 0);
        SystemList& list;
        size_t i    : MAX_SYSTEMS_LOG;
        bool done   : 1;
    public:
        inline system_t& operator*() {return list.systems[i];}
        inline bool operator==(SystemIt const& other) const {return this->i == other.i && this->done == other.done;}
        inline bool operator!=(SystemIt const& other) const {return this->i != other.i && this->done != other.done;}
        SystemIt& operator++();
    };
public:
    SystemIt begin();
    SystemIt end();
};

class System {
public:
    System(float pd);
    inline virtual ~System() {}
    void init(State* state);
    void destroy(State* state);
    void step(float dt, State* state, Keyboard const& kb, Mouse const& mouse, glm::vec2 wmpos, glm::vec2 wmdelt);
protected:
    float const& period;
private:
    float _pd;
    float _t;
    virtual void intr_init(State* state) = 0;
    virtual void intr_destroy(State* state) = 0;
    virtual void intr_step(float dt, State* state, Keyboard const& kb, Mouse const& mouse, glm::vec2 wmpos, glm::vec2 wmdelt) = 0;
};

#endif /* SYSTEM_H */
