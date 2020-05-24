#pragma once
#include "ECS.h"
#include "Matrix4x4.h"

struct TransformComponent : public ECSComponent<TransformComponent>
{
	Matrix4x4 transform;
};