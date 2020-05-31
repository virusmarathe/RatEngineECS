#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "RotateTimerComponent.h"

class EulerRotatorSystem : public BaseECSSystem
{
public:
	EulerRotatorSystem() : BaseECSSystem()
	{
		addComponentType(TransformComponent::ID, 0);
		addComponentType(RotateTimerComponent::ID, 0);
	}

	virtual void updateComponents(float deltaTime, BaseECSComponent** components)
	{
		TransformComponent* transformComponent = (TransformComponent*)components[0];
		RotateTimerComponent* rotateComponent = (RotateTimerComponent*)components[1];

		Vector3 newRot = rotateComponent->oldEulerAngles + (rotateComponent->rotateEulerAngles * deltaTime * rotateComponent->speed);
		if (newRot.x != rotateComponent->oldEulerAngles.x) 
			transformComponent->transform.setRotationX(newRot.x);
		if (newRot.y != rotateComponent->oldEulerAngles.y) 
			transformComponent->transform.setRotationY(newRot.y);
		if (newRot.z != rotateComponent->oldEulerAngles.z) 
			transformComponent->transform.setRotationZ(newRot.z);

		rotateComponent->oldEulerAngles = newRot;
	}
};