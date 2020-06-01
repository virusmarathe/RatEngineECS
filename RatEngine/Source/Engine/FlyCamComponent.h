#pragma once
#include "ECS.h"

struct FlyCamComponent : public ECSComponent<FlyCamComponent>
{
	float speed = 2.0f;
};