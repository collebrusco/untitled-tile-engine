/** 
 * ecs_test.h
 * created 07/24/24 by frank collebrusco 
 */
// #ifndef ECS_TEST_H
#if 0
#define ECS_TEST_H
#include "ECS.h"
#include <stdio.h>

struct TA {
    size_t d[2];
};
struct TB {
    size_t d[5];
};
struct TC {
    size_t d[5];
};
struct TD {
    size_t d[2];
};

struct Tag {}; struct Tag0 {};

Archetype<TA, Tag0> atype;

void ecs_testmain() {
    ECS ecs;
    entID es[8];

    printf("make ents:\n");

    for (int i = 0; i < 8; i++) {
        es[i] = ecs.newEntity();
        ecs.addComp<Tag>(es[i]);
    }
    printf("add comps n tags\n");
    for (int i = 0; i < 8; i+=2) {
        ecs.addComp<TB>(es[i]);
        ecs.addTag<Tag>(es[i]);
        ecs.addTag<Tag0>(es[i+1]);
    }
    for (int i = 0; i < 8; i+=3) {
        ecs.addComp<TA>(es[i]);
    }
    for (int i = 0; i < 8; i+=4) {
        ecs.addComp<TD>(es[i]);
    }

    printf("iterate ta + tag\n");
    for (auto e : ecs.view<TA, Tag>()) {
        printf("e = %llx\n", e);
    }
    
    printf("tagonly:\n\n");
    for (auto e : ecs.view<Tag>()) {
        printf("e = %llx\n", e);
    }

    printf("all:\n\n");
    for (auto e : ecs.view<>()) {
        printf("e = %llx\n", e);
    }

    printf("atype:\n\n");
    for (auto e : ecs.view(atype)) {
        printf("e = %llx\n", e);
    }

    printf("a lotn:\n\n");
    for (auto e : ecs.view<Tag, TD, Tag0, TB>()) {
        printf("e = %llx\n", e);
    }
    

}

#endif /* ECS_TEST_H */
