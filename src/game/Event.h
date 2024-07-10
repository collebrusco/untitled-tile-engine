/** 
 * Event.h
 * created 07/08/24 by frank collebrusco 
 */
#ifndef EVENT_H
#define EVENT_H
#include <flgl.h>
#include <flgl/glm.h>
#include "game/State.h"
#include "game/System.h"

#define MAX_EVENT_SUBS (32)

#define EVENT_DECLARE(name) struct name##EventData; extern Event<name##EventData> name##Event; struct name##EventData 
#define EVENT_DEFINE(name) Event<name##EventData> name##Event

struct __EventIdGetter {
    static uint16_t id_ct;
    template <typename EventInput_t>
    static uint16_t inline get_eid() {static uint16_t id = id_ct++; return id;}
};

template <typename Input_t>
class Event {
    void logw(const char* msg) const;
    void loge(const char* msg) const;
    uint16_t _id;
public:
    uint16_t inline id() const {return _id;}
    void (*subscribers[MAX_EVENT_SUBS])(Input_t*);
    struct {
        uint32_t data[MAX_EVENT_SUBS];
        uint32_t top;
    } freestack;
    uint32_t sub_top;
    Event() {
        sub_top = 0;
        freestack.top = 0;
        _id = __EventIdGetter::get_eid<Input_t>();
        memset(subscribers, 0, MAX_EVENT_SUBS * sizeof(uintptr_t));
        memset(freestack.data, 0, MAX_EVENT_SUBS * sizeof(uint32_t));
    }

    void subscribe(void(*sub)(Input_t*)) {
        if (freestack.top != 0) {
            subscribers[freestack.data[--freestack.top]] = sub;
            return;
        }
        if (sub_top >= MAX_EVENT_SUBS) {
            loge("subscriber overflow, increase size");
            return;
        }
        subscribers[sub_top++] = sub;
    }

    void unsubscribe(void(*sub)(Input_t*)) {
        uint32_t sid = 0;
        while (subscribers[sid] != sub) {sid++; if (sid >= MAX_EVENT_SUBS) {logw("unsub not found"); return;}}
        freestack.data[freestack.top++] = sid;
        subscribers[sid] = 0;
    }

    void fire(Input_t input) {
        for (uint32_t i = 0; i < sub_top; i++) {
            if (subscribers[i]) {
                subscribers[i](&input);
            }
        }
    }
};


#endif /* EVENT_H */
