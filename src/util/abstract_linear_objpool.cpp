#include "abstract_linear_objpool.h"
#include <flgl/logger.h>
LOG_MODULE(ALOP);

/* test main \/ */







// class TBase {
// public:
//     TBase() = default;
//     virtual ~TBase() {LOG_INF("tbase destruct");}
//     size_t a, b, c;
//     virtual void func() {
//         LOG_INF("tbase call");
//     }
// };

// class TSub0 : public TBase {
// public:
//     size_t arr[5];
// };

// class TSub1 : public TBase {
// public:
//     size_t arr[10];
//     TSub1() {
//         LOG_INF("tsub1 construct");
//     }
//     ~TSub1() {
//         LOG_INF("tsub1 destruct");
//     }
//     virtual void func() override {
//         LOG_INF("tsub1 call");
//     }
// };

// class TSub2 : public TBase {
// public:
//     virtual void func() override {
//         LOG_INF("tsub2 call");
//     }
// };

// class TSub3 : public TBase {
//     size_t arr[256];
// public:
//     TSub3(int b) {
//         LOG_INF("tsub3 construct w %d", b);
//     }
//     ~TSub3() {
//         LOG_INF("tsub3 destruct");
//     }
//     virtual void func() override {
//         LOG_INF("tsub3 call");
//     }
// };


// int main() {
//     abstract_linear_objpool<TBase> opool(4096*4);

//     LOG_INF("sizeof Base %d", sizeof(TBase));
//     LOG_INF("sizeof Sub0 %d", sizeof(TSub0));
//     LOG_INF("sizeof Sub1 %d", sizeof(TSub1));
//     LOG_INF("sizeof Sub2 %d", sizeof(TSub2));
//     LOG_INF("sizeof Sub3 %d", sizeof(TSub3));

//     opool.push<TSub0>();
//     opool.push<TSub0>();
//     opool.push<TSub1>();
//     opool.push<TSub2>();
//     opool.push<TSub1>();
//     opool.push<TSub3>(4);
//     opool.push<TSub3>(5);

//     size_t i = 0;
//     for (auto* base : opool) {
//         LOG_INF("in loop i = %d", i++);
//         base->func();
//     }

//     opool.clear();

//     LOG_INF("scope close:");

// }
