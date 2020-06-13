#pragma once
#include "ECS.h"
#include "AABB.h"

struct ColliderComponent : public ECSComponent<ColliderComponent>
{
	// AABB
	AABB aabb;
};