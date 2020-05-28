#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "SimpleMotionComponent.h"
#include "MotionIntegrators.h"

class SimpleMotionSystem : public BaseECSSystem
{
public:
	SimpleMotionSystem() : BaseECSSystem()
	{
		addComponentType(TransformComponent::ID, 0);
		addComponentType(SimpleMotionComponent::ID, 0);
	}

	virtual void updateComponents(float deltaTime, BaseECSComponent** components)
	{
		TransformComponent* transformComponent = (TransformComponent*)components[0];
		SimpleMotionComponent* simpleMotionComponent = (SimpleMotionComponent*)components[1];

		Vector3 newPos = transformComponent->transform.position();
		MotionIntegrators::verlet(newPos, simpleMotionComponent->velocity, simpleMotionComponent->acceleration, deltaTime);
		transformComponent->transform.setTranslation(newPos);
	}
};