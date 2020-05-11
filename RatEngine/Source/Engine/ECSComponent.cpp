#include "ECSComponent.h"

std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>* BaseECSComponent::componentInfo;

uint32_t BaseECSComponent::registerComponentType(ECSComponentCreateFunction createFn, ECSComponentFreeFunction freeFn,
												 size_t size)
{
	if (componentInfo == nullptr)
	{
		componentInfo = new std::vector<std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>>();
	}
	uint32_t componentID = (uint32_t)componentInfo->size();
	componentInfo->push_back(std::tuple<ECSComponentCreateFunction, ECSComponentFreeFunction, size_t>(createFn, freeFn, size));
	return componentID;
}