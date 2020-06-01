#include "ECSSystem.h"

bool BaseECSSystem::isValid()
{
	for (size_t i = 0; i < componentFlags.size(); i++)
	{
		if ((componentFlags[i] && BaseECSSystem::FLAG_OPTIONAL) == 0)
		{
			return true;
		}
	}

	return false;
}

bool ECSSystemList::removeSystem(BaseECSSystem& system)
{
	for (size_t i = 0; i < m_Systems.size(); i++)
	{
		if (&system == m_Systems[i])
		{
			m_Systems[i]->cleanup();
			m_Systems.erase(m_Systems.begin() + i);
			return true;
		}
	}

	return false;
}