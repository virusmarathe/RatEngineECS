#include "InputSystem.h"
#include <Windows.h>

InputSystem::InputSystem()
{
}

InputSystem::~InputSystem()
{
}

void InputSystem::update()
{
	if (GetKeyboardState(m_Keys))
	{
		for (unsigned int i = 0; i < 256; i++)
		{
			if (m_Keys[i] & 0x80) // key is down
			{
				for (const auto& listener : m_Listeners)
				{
					listener->onKeyDown(i);
				}
			}
			else // key is up
			{
				if (m_LastFrameKeys[i] != m_Keys[i])
				{
					for (const auto& listener : m_Listeners)
					{
						listener->onKeyUp(i);
					}
				}
			}
		}

		memcpy(m_LastFrameKeys, m_Keys, sizeof(unsigned char) * 256);
	}
}

void InputSystem::addListener(InputListener* listener)
{
	m_Listeners.insert(listener);
}

void InputSystem::removeListener(InputListener* listener)
{
	m_Listeners.erase(listener);
}

InputSystem* InputSystem::get()
{
	static InputSystem system;
	return &system;
}
