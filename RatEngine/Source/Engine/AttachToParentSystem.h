#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "AttachToParentComponent.h"

class AttachToParentSystem : public BaseECSSystem
{
public:
	AttachToParentSystem(ECS * ecs) : BaseECSSystem(), ecsRef(ecs)
	{
		addComponentType(TransformComponent::ID, 0);
		addComponentType(AttachToParentComponent::ID, 0);
	}

	virtual void updateComponents(float deltaTime, BaseECSComponent** components)
	{
		TransformComponent* transformComponent = (TransformComponent*)components[0];
		AttachToParentComponent* attachToParentComp = (AttachToParentComponent*)components[1];

		Vector3 parentPos = ecsRef->getComponent<TransformComponent>(attachToParentComp->parent)->transform.position();

		transformComponent->transform.setTranslation(parentPos + attachToParentComp->positionOffset);
	}

	ECS * ecsRef;
};