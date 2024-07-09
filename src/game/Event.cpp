#include "Event.h"
#include "Events.h"
#include <flgl/logger.h>
LOG_MODULE(event);

EVENT_DEFINE(ActorMove);



/* define more events here ... */







template <typename Input_t>
void Event<Input_t>::loge(const char* msg) const {
    LOG_ERR("%s", msg);
}
template <typename Input_t>
void Event<Input_t>::logw(const char* msg) const {
    LOG_WRN("%s", msg);
}

/* hacked little test main \/ */

// #include <flgl/logger.h>
// LOG_MODULE(event_test);
// int subfs[8];

// void s0(ActorMoveEventData* d) {
//     subfs[0] = 1;
// }
// void s1(ActorMoveEventData* d) {
//     subfs[1] = 1;
// }
// void s2(ActorMoveEventData* d) {
//     subfs[2] = 1;
// }
// void s3(ActorMoveEventData* d) {
//     subfs[3] = 1;
// }
// void s4(ActorMoveEventData* d) {
//     subfs[4] = 1;
// }
// void s5(ActorMoveEventData* d) {
//     subfs[5] = 1;
// }
// void s6(ActorMoveEventData* d) {
//     subfs[6] = 1;
// }
// void s7(ActorMoveEventData* d) {
//     subfs[7] = 1;
// }

// void reset() {
//     memset(subfs, 0, sizeof(int)*8);
// }
// #include <unordered_map>
// using namespace std;
// unordered_map<void*, const char*> fns;

// void dump() {
//     LOG_ERR("\n");
//     LOG_ERR("Subs: (top = %d)", ActorMoveEvent.sub_top);
//     LOG_ERR("subs[0]: %s", fns[(void*)ActorMoveEvent.subscribers[0]]);
//     LOG_ERR("subs[1]: %s", fns[(void*)ActorMoveEvent.subscribers[1]]);
//     LOG_ERR("subs[2]: %s", fns[(void*)ActorMoveEvent.subscribers[2]]);
//     LOG_ERR("subs[3]: %s", fns[(void*)ActorMoveEvent.subscribers[3]]);
//     LOG_ERR("subs[4]: %s", fns[(void*)ActorMoveEvent.subscribers[4]]);
//     LOG_ERR("subs[5]: %s", fns[(void*)ActorMoveEvent.subscribers[5]]);
//     LOG_ERR("subs[6]: %s", fns[(void*)ActorMoveEvent.subscribers[6]]);
//     LOG_ERR("subs[7]: %s", fns[(void*)ActorMoveEvent.subscribers[7]]);

//     LOG_ERR("freelist: (top = %d)", ActorMoveEvent.freestack.top);
//     for (int i = 0; i < ActorMoveEvent.freestack.top; i++) {
//         LOG_ERR("freestk[%d] = %d", i, ActorMoveEvent.freestack.data[i]);
//     }
//     LOG_ERR("\n");
// }
// int main() {
//     fns[(void*)s0] = "s0";
//     fns[(void*)s1] = "s1";
//     fns[(void*)s2] = "s2";
//     fns[(void*)s3] = "s3";
//     fns[(void*)s4] = "s4";
//     fns[(void*)s5] = "s5";
//     fns[(void*)s6] = "s6";
//     fns[(void*)s7] = "s7";
//     ActorMoveEventData in = {.actor = 555, .oldpos = {0.f,0.f}, .newpos = {1.f,1.f}};
//     reset();

//     LOG_ERR("sub s0:");

//     ActorMoveEvent.subscribe(s0);
//     ActorMoveEvent.fire(in);
//     dump();

//     assert(subfs[0] == 1);
//     assert(subfs[1] == 0);
//     assert(subfs[2] == 0);
//     assert(subfs[3] == 0);
//     assert(subfs[4] == 0);
//     assert(subfs[5] == 0);
//     assert(subfs[6] == 0);
//     assert(subfs[7] == 0);

//     reset();

//     LOG_ERR("sub s1 & 2");

//     ActorMoveEvent.subscribe(s1);
//     ActorMoveEvent.subscribe(s2);
//     ActorMoveEvent.fire(in);
//     dump();

//     assert(subfs[0] == 1);
//     assert(subfs[1] == 1);
//     assert(subfs[2] == 1);
//     assert(subfs[3] == 0);
//     assert(subfs[4] == 0);
//     assert(subfs[5] == 0);
//     assert(subfs[6] == 0);
//     assert(subfs[7] == 0);

//     reset();

//     LOG_ERR("unsub 1");

//     ActorMoveEvent.unsubscribe(s1);
//     ActorMoveEvent.fire(in);
//     dump();

//     assert(ActorMoveEvent.freestack.top == 1);
//     assert(ActorMoveEvent.freestack.data[0] == 1);
//     assert(ActorMoveEvent.subscribers[1] == 0);

//     assert(subfs[0] == 1);
//     assert(subfs[1] == 0);
//     assert(subfs[2] == 1);
//     assert(subfs[3] == 0);
//     assert(subfs[4] == 0);
//     assert(subfs[5] == 0);
//     assert(subfs[6] == 0);
//     assert(subfs[7] == 0);

//     LOG_ERR("sub 7");

//     ActorMoveEvent.subscribe(s7);
//     dump();
//     assert(ActorMoveEvent.sub_top == 3);
//     assert(ActorMoveEvent.freestack.top == 0);
//     assert(ActorMoveEvent.subscribers[1] == s7);

//     ActorMoveEvent.unsubscribe(s0);
//     ActorMoveEvent.unsubscribe(s1);
//     ActorMoveEvent.unsubscribe(s2);
//     ActorMoveEvent.unsubscribe(s3);
//     ActorMoveEvent.unsubscribe(s4);
//     ActorMoveEvent.unsubscribe(s5);
//     ActorMoveEvent.unsubscribe(s6);
//     ActorMoveEvent.unsubscribe(s7);
//     ActorMoveEvent.subscribe(s0);
//     ActorMoveEvent.subscribe(s1);
//     ActorMoveEvent.subscribe(s2);
//     ActorMoveEvent.subscribe(s3);
//     ActorMoveEvent.subscribe(s4);
//     ActorMoveEvent.subscribe(s5);
//     ActorMoveEvent.subscribe(s6);
//     ActorMoveEvent.subscribe(s7);

//     LOG_ERR("suball:");
//     dump();

//     LOG_ERR("rem a few:");
//     ActorMoveEvent.unsubscribe(s0);
//     ActorMoveEvent.unsubscribe(s5);
//     ActorMoveEvent.unsubscribe(s6);
//     ActorMoveEvent.unsubscribe(s2);

//     dump();

//     LOG_ERR("add back 2 6 5 0:");
//     ActorMoveEvent.subscribe(s2);
//     ActorMoveEvent.subscribe(s6);
//     ActorMoveEvent.subscribe(s5);
//     ActorMoveEvent.subscribe(s0);

//     dump();

//     LOG_ERR("ALL PASS! ALL GOOD!");
// }

