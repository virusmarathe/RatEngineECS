#include "ECS.h"
#include "Debug.h"
#include "RMath.h"

ECS::~ECS()
{
	for (const auto& component : m_Components)
	{ 
		size_t typeSize = BaseECSComponent::getTypeSize(component.first);
		ECSComponentFreeFunction freeFn = BaseECSComponent::getTypeFreeFunction(component.first);
		for (size_t i = 0; i < component.second.size(); i+= typeSize)
		{
			freeFn((BaseECSComponent*)&component.second[i]);
		}
	}

	for (size_t i = 0; i < m_Entities.size(); i++)
	{
		delete m_Entities[i];
	}
}

EntityHandle ECS::makeEntity(BaseECSComponent** components, const uint32_t* componentIDs, size_t numComponents)
{
	std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>* newEntity = new std::pair<uint32_t, std::vector<std::pair<uint32_t, uint32_t>>>();
	EntityHandle handle = (EntityHandle)newEntity;

	for (uint32_t i = 0; i < numComponents; i++)
	{
		if (!BaseECSComponent::isTypeValid(componentIDs[i]))
		{
			delete newEntity;
			DEBUG_LOG("ECS", LOG_ERROR, "%u id is not a valid component type!", componentIDs[i]);
			return NULL_ENTITY_HANDLE;
		}
		addComponentInternal(handle, newEntity->second, componentIDs[i], components[i]);
	}

	newEntity->first = (uint32_t)m_Entities.size();
	m_Entities.push_back(newEntity);
	return handle;
}

void ECS::removeEntity(EntityHandle handle)
{
	std::vector<std::pair<uint32_t, uint32_t>>& entity = handleToEntity(handle);
	for (size_t i = 0; i < entity.size(); i++)
	{
		deleteComponent(entity[i].first, entity[i].second);
	}

	uint32_t destIndex = handleToEntityIndex(handle);
	uint32_t srcIndex = (uint32_t)(m_Entities.size() - 1);
	delete m_Entities[destIndex];
	m_Entities[destIndex] = m_Entities[srcIndex];
	m_Entities[destIndex]->first = destIndex;
	m_Entities.pop_back();
}

void ECS::updateSystems(ECSSystemList& systems, float deltaTime)
{
	std::vector<BaseECSComponent*> componentParam;
	std::vector<std::vector<uint8_t>*> componentArrays;
	for (size_t i = 0; i < systems.size(); i++)
	{
		const std::vector<uint32_t>& componentTypes = systems[i]->getComponentTypes();

		if (componentTypes.size() == 1)
		{
			size_t typeSize = BaseECSComponent::getTypeSize(componentTypes[0]);
			std::vector<uint8_t>& componentArray = m_Components[componentTypes[0]];
			for (size_t j = 0; j < componentArray.size(); j += typeSize)
			{
				BaseECSComponent* comp = (BaseECSComponent*)&componentArray[j];
				systems[i]->updateComponents(deltaTime, &comp);
			}
		}
		else
		{
			updateSystemWithMultipleComponents(systems, i, deltaTime, componentTypes, componentParam, componentArrays);
		}
	}
}

void ECS::addComponentInternal(EntityHandle handle, std::vector<std::pair<uint32_t, uint32_t>>& entity, uint32_t componentID, BaseECSComponent* component)
{
	ECSComponentCreateFunction createFn = BaseECSComponent::getTypeCreateFunction(componentID);
	std::pair<uint32_t, uint32_t> newPair;
	newPair.first = componentID;
	newPair.second = createFn(m_Components[componentID], handle, component);
	entity.push_back(newPair);
}

bool ECS::removeComponentInternal(EntityHandle handle, uint32_t componentID)
{
	std::vector<std::pair<uint32_t, uint32_t>>& entityComponents = handleToEntity(handle);
	for (size_t i = 0; i < entityComponents.size(); i++)
	{
		if (entityComponents[i].first == componentID)
		{
			deleteComponent(entityComponents[i].first, entityComponents[i].second);
			size_t srcIndex = entityComponents.size() - 1;
			size_t dstIndex = i;
			entityComponents[dstIndex] = entityComponents[srcIndex];
			entityComponents.pop_back();
			return true;
		}
	}

	return false;
}

void ECS::deleteComponent(uint32_t componentID, uint32_t componentIndex)
{
	std::vector<uint8_t>& data = m_Components[componentID];
	ECSComponentFreeFunction freeFn = BaseECSComponent::getTypeFreeFunction(componentID);
	size_t typeSize = BaseECSComponent::getTypeSize(componentID);

	uint32_t srcIndex = (uint32_t)(data.size() - typeSize);

	BaseECSComponent* dstComponent = (BaseECSComponent*)&data[componentIndex];
	BaseECSComponent* srcComponent = (BaseECSComponent*)&data[srcIndex];
	freeFn(dstComponent);

	if (componentIndex == srcIndex)
	{
		data.resize(srcIndex);
		return;
	}

	memcpy(dstComponent, srcComponent, typeSize);

	// need to repoint the entities that used to point to srcIndex to use compIndex
	std::vector<std::pair<uint32_t, uint32_t>>& srcComponents = handleToEntity(srcComponent->entity);
	for (size_t i = 0; i < srcComponents.size(); i++)
	{
		if (srcComponents[i].first == componentID && srcComponents[i].second == srcIndex)
		{
			srcComponents[i].second = componentIndex;
			break;
		}
	}

	data.resize(srcIndex);
}

BaseECSComponent* ECS::getComponentInternal(std::vector<std::pair<uint32_t, uint32_t>>& entityComponents,
											std::vector<uint8_t>& componentArray, uint32_t componentID)
{
	for (size_t i = 0; i < entityComponents.size(); i++)
	{
		if (entityComponents[i].first == componentID)
		{
			return (BaseECSComponent*)&componentArray[entityComponents[i].second];
		}
	}

	return nullptr;
}

void ECS::updateSystemWithMultipleComponents(ECSSystemList& systems, size_t index, float deltaTime,
											 const std::vector<uint32_t>& componentTypes,
											 std::vector<BaseECSComponent*>& componentParam,
											 std::vector<std::vector<uint8_t>*>& componentArrays)
{
	const std::vector<uint32_t>& componentFlags = systems[index]->getComponentFlags();

	componentParam.resize(RMath::max(componentParam.size(), componentTypes.size()));
	componentArrays.resize(RMath::max(componentArrays.size(), componentTypes.size()));

	for (size_t i = 0; i < componentTypes.size(); i++)
	{
		componentArrays[i] = &m_Components[componentTypes[i]];
	}

	uint32_t minSizeIndex = findLeastCommonComponent(componentTypes, componentFlags);

	size_t typeSize = BaseECSComponent::getTypeSize(componentTypes[minSizeIndex]);
	std::vector<uint8_t>& minIndexArray = *componentArrays[minSizeIndex];
	for (size_t i = 0; i < minIndexArray.size(); i += typeSize)
	{
		componentParam[minSizeIndex] = (BaseECSComponent*)&minIndexArray[i];
		std::vector<std::pair<uint32_t, uint32_t>>& entityComponents = handleToEntity(componentParam[minSizeIndex]->entity);
		bool isValid = true;
		for (size_t j = 0; j < componentTypes.size(); j++)
		{
			if (j == minSizeIndex)
			{
				continue;
			}
			
			componentParam[j] = getComponentInternal(entityComponents, *componentArrays[j], componentTypes[j]);
			if (componentParam[j] == nullptr && ((componentFlags[j] & BaseECSSystem::FLAG_OPTIONAL) == 0))
			{
				isValid = false;
				break;
			}
		}

		if (isValid)
		{
			systems[index]->updateComponents(deltaTime, &componentParam[0]);
		}
	}
}

uint32_t ECS::findLeastCommonComponent(const std::vector<uint32_t>& componentTypes,
									   const std::vector<uint32_t>& componentFlags)
{
	uint32_t minSize = (uint32_t)-1;
	uint32_t minIndex = 0;

	for (size_t i = 0; i < componentTypes.size(); i++)
	{
		if (componentFlags[i] & BaseECSSystem::FLAG_OPTIONAL)
		{
			continue;
		}
		size_t typeSize = BaseECSComponent::getTypeSize(componentTypes[i]);
		uint32_t size = (uint32_t)(m_Components[componentTypes[i]].size() / typeSize);

		if (size <= minSize)
		{
			minSize = size;
			minIndex = (uint32_t)i;
		}
	}

	return minIndex;
}
