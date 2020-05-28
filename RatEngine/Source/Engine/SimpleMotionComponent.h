#pragma once
#include "ECS.h"
#include "Vector3.h"

struct SimpleMotionComponent : public ECSComponent<SimpleMotionComponent>
{
	Vector3 velocity = Vector3(0.0f, 0.0f, 0.0f);
	Vector3 acceleration = Vector3(0.0f, 0.0f, 0.0f);
};