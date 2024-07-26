#include "util/abstract_linear_objpool.h"
#include "util/abstract_freelist_objpool.h"
#include <flgl/logger.h>
LOG_MODULE(utest);

/**
 * ================ LINEAR ================
 */

class TBase {
public:
    TBase() = default;
    virtual ~TBase() {LOG_INF("tbase destruct");}
    size_t a, b, c;
    virtual void func() {
        LOG_INF("tbase call");
    }
};

class TSub0 : public TBase {
public:
    size_t arr[5];
};

class TSub1 : public TBase {
public:
    size_t arr[10];
    TSub1() {
        LOG_INF("tsub1 construct");
    }
    ~TSub1() {
        LOG_INF("tsub1 destruct");
    }
    virtual void func() override {
        LOG_INF("tsub1 call");
    }
};

class TSub2 : public TBase {
public:
    virtual void func() override {
        LOG_INF("tsub2 call");
    }
};

class TSub3 : public TBase {
    char arr[256];
public:
    TSub3(int b) {
        LOG_INF("tsub3 construct w %d", b);
    }
    ~TSub3() {
        LOG_INF("tsub3 destruct");
    }
    virtual void func() override {
        LOG_INF("tsub3 call");
    }
};


static void run_alop_test() {
    abstract_linear_objpool<TBase> opool(4096*4);

    LOG_INF("sizeof Base %d", sizeof(TBase));
    LOG_INF("sizeof Sub0 %d", sizeof(TSub0));
    LOG_INF("sizeof Sub1 %d", sizeof(TSub1));
    LOG_INF("sizeof Sub2 %d", sizeof(TSub2));
    LOG_INF("sizeof Sub3 %d", sizeof(TSub3));

    opool.push<TSub0>();
    opool.push<TSub0>();
    opool.push<TSub1>();
    opool.push<TSub2>();
    opool.push<TSub1>();
    opool.push<TSub3>(4);
    opool.push<TSub3>(5);

    size_t i = 0;
    for (auto* base : opool) {
        LOG_INF("in loop i = %d", i++);
        base->func();
    }

    opool.clear();

    LOG_INF("scope close:");

}


/**
 * ================ FREE ================
 */
#define SLOTS 8
using afop = abstract_freelist_objpool<TBase, (1<<9)-1, SLOTS>;

static void psubs(afop& pool) {
    uint16_t i = 0;
    while (i < SLOTS) {
        LOG_INF("sub[%d](%d) = %p", i, pool.is_valid(i), pool.at(i));
        i++;
    }
}
#define NL(...) LOG_INF("");
static void run_afop_test() {
    afop pool;
    psubs(pool);
    NL();
    TBase *a, *b, *c, *d, *e, *f;
    a = pool.push<TSub0>();
    b = pool.push<TSub1>();
    c = pool.push<TSub1>();
    d = pool.push<TSub2>();
    e = pool.push<TSub3>(99);

    psubs(pool); NL();

    for (auto* b : pool) {
        LOG_INF("iter!(%d): %p", pool.get_i(b), b);
        b->func();
    }

    pool.remove(c); pool.remove(d);

    psubs(pool); NL();

    pool.push<TSub2>(); pool.push<TSub2>(); pool.push<TSub2>();

    psubs(pool); NL();

    for (auto* b : pool) {
        LOG_INF("iter!(%d): %p", pool.get_i(b), b);
        b->func();
    }

    LOG_INF("scope end:");

}


#include "tests/ecs_test.h"

int main() {
    // run_alop_test(); LOG_INF("");
    // run_afop_test();
    ecs_testmain();
}
