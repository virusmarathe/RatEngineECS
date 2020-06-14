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

void InteractionWorld::processInteractions(float deltaTime)
{
	removeEntities();
	int axis = findHighestVarianceAxis();
}

void InteractionWorld::removeEntities()
{
	for (EntityHandle toRemoveEntity : m_EntitesToRemove)
	{
		for (int i = 0; i < m_Entities.size(); i++)
		{
			if (m_Entities[i] == toRemoveEntity)
			{
				EntityHandle temp = m_Entities[i];
				m_Entities[i] = m_Entities[m_Entities.size() - 1];
				m_Entities[m_Entities.size() - 1] = temp;
				m_Entities.pop_back();
				break;
			}
		}
	}

	m_EntitesToRemove.clear();
}

int InteractionWorld::findHighestVarianceAxis()
{
	return 0;
}
