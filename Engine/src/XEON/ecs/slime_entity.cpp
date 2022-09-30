#include <xeonpch.h>

#include <XEON/ecs/slime_entity.h>

Entity CreateSlimeEntity(EntityStorage& storage, const glm::vec3& position) {
	is_unique<int, double>::value;
	return storage.newEntity()
		.emplaceComponent<PositionComponent>(position)
		.emplaceComponent<NameComponent>("Slime")
		//.emplaceComponent<int>(0)							// Doesn't compile because `int` is not a Component.
		//.emplaceComponent<PositionComponent>(position)	// Doesn't compile because adding duplicate Components is not allowed.
		.build();

}