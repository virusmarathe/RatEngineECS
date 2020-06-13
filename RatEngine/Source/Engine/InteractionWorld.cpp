#include "InteractionWorld.h"

void InteractionWorld::onMakeEntity(EntityHandle handle)
{
	m_Entities.push_back(handle);
}

void InteractionWorld::onRemoveEntity(EntityHandle handle)
{
	m_EntitesToRemove.push_back(handle);
}

void InteractionWorld::onAddComponent(EntityHandle handle, uint32_t id)
{
	if (id == TransformComponent::ID)
	{
		if (m_ECS.getComponent<ColliderComponent>(handle) != nullptr)
		{
			m_Entities.push_back(handle);
		}
	}
	else if (id == ColliderComponent::ID)
	{
		if (m_ECS.getComponent<TransformComponent>(handle) != nullptr)
		{
			m_Entities.push_back(handle);
		}
	}
}

void InteractionWorld::onRemoveComponent(EntityHandle handle, uint32_t id)
{
	if (id == TransformComponent::ID || id == ColliderComponent::ID)
	{
		m_EntitesToRemove.push_back(handle);
	}
}
