#include "Entity.h"

void Entity::config_for_sprite(entID e, ECS &ecs, Sprite sprite) {
    auto* obj = ecs.tryGetComp<c_Object>(e);
    auto* csa = ecs.tryGetComp<c_EntRenderEntry>(e);
    if (!obj)
        obj = &ecs.addComp<c_Object>(e);
    if (!csa)
        csa = &ecs.addComp<c_EntRenderEntry>(e);
    (*(csa)) = sprite.at;
    obj->scale = sprite.size;
}
