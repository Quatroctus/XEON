#pragma once

#include <XEON/ecs/entity_storage.hpp>
#include<XEON/ecs/component.h>

#include <glm/glm.hpp>

constexpr size_t SLIME_ENTITY_TYPE = 1;
Entity CreateSlimeEntity(EntityStorage& storage, const glm::vec3& position);
