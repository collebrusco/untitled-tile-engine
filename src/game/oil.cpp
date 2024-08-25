#include "oil.h"
#include <flgl/logger.h>
LOG_MODULE(oil);

void c_oilrig::place(ECS &ecs, SurfaceTile &st) {
    if (!ecs.entityValid(st.ent)) st.ent = ecs.newEntity();
    ecs.addComp<c_oilrig>(st.ent).reserve = 100.f;
}

#define EXTRACT_RATE (1.f)

void c_oilrig::execute(ECS& ecs, float const dt) {
    for (auto e : ecs.view<c_oilrig>()) {
        float& rsv = ecs.getComp<c_oilrig>(e).reserve;
        rsv -= EXTRACT_RATE * dt;
        if (rsv < 0.f) rsv = 0.f;
        LOG_INF("res down to %.0f", rsv);
    }
}
