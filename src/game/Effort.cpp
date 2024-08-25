#include "Effort.h"
#include "data/components.h"
#include <flgl/logger.h>
LOG_MODULE(eff);

void c_RotationEffort::execute(ECS &ecs, float const dt) {
    for (auto e : ecs.view<c_RotationEffort>()) {
        auto& re = ecs.getComp<c_RotationEffort>(e);
        re.r.step(re.Kp, re.Ki, re.Kl, dt);
        auto* o = ecs.tryGetComp<c_Object>(e);
        if (o) o->rot = re.r.val;
    }
}
