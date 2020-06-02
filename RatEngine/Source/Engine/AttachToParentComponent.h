#pragma once
#include "ECS.h"
#include "Matrix4x4.h"

struct AttachToParentComponent : public ECSComponent<AttachToParentComponent>
{
	EntityHandle parent;
	Vector3 positionOffset = Vector3();
};