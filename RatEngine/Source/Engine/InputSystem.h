#pragma once
#include "InputListener.h"
#include <unordered_set>

class InputSystem
{
public:
	InputSystem();
	~InputSystem();

	void update();
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	static InputSystem* get();

private:
	std::unordered_set<InputListener*> m_Listeners;
	unsigned char m_Keys[256] = {};
	unsigned char m_LastFrameKeys[256] = {};
};

