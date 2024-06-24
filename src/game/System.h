/** 
 * System.h
 * created 06/22/24 by frank collebrusco 
 */
#ifndef SYSTEM_H
#define SYSTEM_H
#include <flgl.h>
#include "game/State.h"

typedef void(*system_init_f)(State* state);
typedef void(*system_dest_f)(State* state);
typedef void(*system_step_f)(float dt, State* state, Keyboard const& kb, Mouse const& mouse, glm::vec2 wmpos, glm::vec2 wmdelt);

struct system_t {
    system_init_f init;
    system_step_f step;
    system_dest_f dest;
    float period_s;
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
        inline system_t operator*() {return list.systems[i];}
        inline bool operator==(SystemIt const& other) const {return this->i == other.i && this->done == other.done;}
        inline bool operator!=(SystemIt const& other) const {return this->i != other.i && this->done != other.done;}
        SystemIt& operator++();
    };
public:
    SystemIt begin();
    SystemIt end();
};


#endif /* SYSTEM_H */
