#pragma once
#include "ECS.h"
#include "Matrix4x4.h"

struct RotateTimerComponent : public ECSComponent<RotateTimerComponent>
{
	float speed;
	Vector3 oldEulerAngles;
	Vector3 rotateEulerAngles;
};