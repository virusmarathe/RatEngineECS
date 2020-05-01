#pragma once

#include <Windows.h>

class Window
{
public:
	Window();
	~Window();

	// Initialize Window
	bool init();

	bool broadcast();

	// Release Window
	bool release();

	bool isRunning() { return m_running; }

	RECT getClientWindowRect();
	void setHWND(HWND hwnd) { m_hwnd = hwnd; }

	// events
	virtual void onCreate() = 0;
	virtual void onUpdate() = 0;
	virtual void onDestroy();
	virtual void onFocus();
	virtual void onKillFocus();

protected:
	HWND m_hwnd;
	bool m_running;
};

