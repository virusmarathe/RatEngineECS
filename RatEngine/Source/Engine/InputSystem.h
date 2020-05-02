#pragma once
#include "InputListener.h"
#include <unordered_set>
#include "Point.h"

class InputSystem
{
private:
	InputSystem();
	~InputSystem();

public:
	void update();
	void addListener(InputListener* listener);
	void removeListener(InputListener* listener);

	void setCursorPosition(const Point& pos);

	void showCursor(bool show);

	static InputSystem* get();
	static void create();
	static void release();

private:
	std::unordered_set<InputListener*> m_Listeners;
	unsigned char m_Keys[256] = {};
	unsigned char m_LastFrameKeys[256] = {};
	Point m_OldMousePos;
	bool m_IsFirstFrame = true;
	static InputSystem* m_InputSystem;
};

