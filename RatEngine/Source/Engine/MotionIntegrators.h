#pragma once
#include "Vector3.h"

class MotionIntegrators
{
public:

	static void verlet(Vector3& pos, Vector3& vel, Vector3& acc, float delta)
	{
		float halfDelta = delta * 0.5f;
		pos = pos + (vel * halfDelta);
		vel = vel + (acc * delta);
		pos = pos + (vel * halfDelta);
	}

	static void modifiedEuler(Vector3& pos, Vector3& vel, Vector3& acc, float delta)
	{
		vel = vel + (acc * delta);
		pos = pos + (vel * delta);
	}
};