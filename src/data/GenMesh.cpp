#include "GenMesh.h"
#include <flgl/logger.h>
LOG_MODULE(gm);

void c_GenMesh::execute(float const dt, float const t, ECS &ecs) {
    for (auto e : ecs.view<c_GenMesh>()) {
        ecs.getComp<c_GenMesh>(e).get().step(ecs.getComp<c_Object>(e), dt, t);
    }
}

glm::mat4 GenMesh::model(c_Object const &obj) const {
    return genModelMat2d(obj.pos, obj.rot, obj.scale, obj.anc);
}
