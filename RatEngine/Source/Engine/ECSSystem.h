#pragma once
#include "ECSComponent.h"

class BaseECSSystem
{
public:
	enum
	{
		FLAG_OPTIONAL = 1,
	};
	BaseECSSystem() {}
	virtual void updateComponents(float deltaTime, BaseECSComponent** components) {}
	virtual void cleanup() {}
	const std::vector<uint32_t>& getComponentTypes()
	{
		return componentTypes;
	}
	const std::vector<uint32_t>& getComponentFlags()
	{
		return componentFlags;
	}
	bool isValid();
protected:
	void addComponentType(uint32_t componentType, uint32_t componentFlag)
	{
		componentTypes.push_back(componentType);
		componentFlags.push_back(componentFlag);
	}
private:
	std::vector<uint32_t> componentTypes;
	std::vector<uint32_t> componentFlags;
};

class ECSSystemList
{
public:
	inline bool addSystem(BaseECSSystem& system)
	{
		if (!system.isValid())
		{
			return false;
		}
		m_Systems.push_back(&system);

		return true;
	}

	inline size_t size()
	{
		return m_Systems.size();
	}

	inline BaseECSSystem* operator[](size_t index)
	{
		return m_Systems[index];
	}

	bool removeSystem(BaseECSSystem& system);

private:
	std::vector<BaseECSSystem*> m_Systems;
};