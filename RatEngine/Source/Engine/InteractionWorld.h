#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "ColliderComponent.h"

class InteractionWorld : public ECSListener
{
public:
	InteractionWorld(ECS& ecs) : ECSListener(), m_ECS(ecs)
	{
		addComponentID(TransformComponent::ID);
		addComponentID(ColliderComponent::ID);
	}

	virtual void onMakeEntity(EntityHandle handle);
	virtual void onRemoveEntity(EntityHandle handle);
	virtual void onAddComponent(EntityHandle handle, uint32_t id);
	virtual void onRemoveComponent(EntityHandle handle, uint32_t id);

private:
	ECS& m_ECS;
	std::vector<EntityHandle> m_Entities;
	std::vector<EntityHandle> m_EntitesToRemove;
};

