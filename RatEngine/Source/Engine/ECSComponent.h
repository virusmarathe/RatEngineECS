#pragma once
#include <cstdint>
#include <vector>
#include <tuple>

struct BaseECSComponent;
typedef void* EntityHandle;
#define NULL_ENTITY_HANDLE nullptr
typedef uint32_t(*ECSComponentCreateFunction)(std::vector<uint8_t>& memory, EntityHandle entity, BaseECSComponent* comp);
typedef void (*ECSComponentFreeFunction)(BaseECSComponent* comp);

struct BaseECSComponent
{
public:
	static uint32_t registerComponentType(ECSComponentCreateFunction createFn, ECSComponentFreeFunction freeFn, size_t size);
	EntityHandle entity = NULL_ENTITY_HANDLE;

	inline static ECSComponentCreateFunction getTypeCreateFunction(uint32_t ID)
	{
		return std::get<0>((*componentInfo)[ID]);
	}

	inline static ECSComponentFreeFunction getTypeFreeFunction(uint32_t ID)
	{
		return std::get<1>((*componentInfo)[ID]);
	}

	inline static size_t getTypeSize(uint32_t ID)
	{
		return std::get<2>((*componentInfo)[ID]);
	}

	inline static bool isTypeValid(uint32_t ID)
	{
		return ID < componentInfo->size();
	}

private:
	static std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>* componentInfo;
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
	uint32_t index = (uint32_t)memory.size();
	memory.resize(index + Component::SIZE);
	Component* component = new(&memory[index]) Component(*(Component*)comp);
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
const uint32_t ECSComponent<T>::ID(BaseECSComponent::registerComponentType(ECSComponentCreate<T>, ECSComponentFree<T>, sizeof(T)));

template<typename T>
const size_t ECSComponent<T>::SIZE(sizeof(T));

template<typename T>
const ECSComponentCreateFunction ECSComponent<T>::CREATE_FUNCTION(ECSComponentCreate<T>);

template<typename T>
const ECSComponentFreeFunction ECSComponent<T>::FREE_FUNCTION(ECSComponentFree<T>);