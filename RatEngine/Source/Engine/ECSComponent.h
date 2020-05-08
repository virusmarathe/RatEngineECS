#pragma once
#include <cstdint>
#include <vector>

struct BaseECSComponent;
typedef void* EntityHandle;
#define NULL_ENTITY_HANDLE nullptr
typedef uint32_t(*ECSComponentCreateFunction)(std::vector<uint8_t>& memory, EntityHandle entity, BaseECSComponent* comp);
typedef void (*ECSComponentFreeFunction)(BaseECSComponent* comp);

struct BaseECSComponent
{
	static uint32_t nextID();
	EntityHandle entity = NULL_ENTITY_HANDLE;
};

template<typename T>
struct ECSComponent : public BaseECSComponent
{
	static const ECSComponentCreateFunction CREATE_FUNCTION;
	static const ECSComponentFreeFunction FREE_FUNCTION;
	static const uint32_t ID;
	static const size_t SIZE;
};

template<typename Component>
uint32_t ECSComponentCreate(std::vector<uint8_t>& memory, EntityHandle entity, BaseECSComponent* comp)
{
	uint32_t index = memory.size();
	memory.resize(index + Component::SIZE);
	Component* component = new(memory[index]) Component(*(Component*)comp);
	component->entity = entity;
	return index;
}

template<typename Component>
void ECSComponentFree(BaseECSComponent* comp)
{
	Component* component = (Component*)comp;
	component->~Component();
}

template<typename T>
const uint32_t ECSComponent<T>::ID(BaseECSComponent::nextID());

template<typename T>
const size_t ECSComponent<T>::SIZE(sizeof(T));

template<typename T>
const ECSComponentCreateFunction ECSComponent<T>::CREATE_FUNCTION(ECSComponentCreate<T>);

template<typename T>
const ECSComponentFreeFunction ECSComponent<T>::FREE_FUNCTION(ECSComponentFree<T>);

class TestECSComponent : public ECSComponent<TestECSComponent>
{
public:
	float x;
	float y;
};