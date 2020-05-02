#pragma once
#include "Point.h"

class InputListener
{
public:
	InputListener()
	{
	}
	~InputListener()
	{
	}

	virtual void onKeyDown(int key) = 0;
	virtual void onKeyUp(int key) = 0;

	virtual void onMouseMove(const Point& mousePos) = 0;
	virtual void onLeftMouseDown(const Point& mousePos) = 0;
	virtual void onLeftMouseUp(const Point& mousePos) = 0;
	virtual void onRightMouseDown(const Point& mousePos) = 0;
	virtual void onRightMouseUp(const Point& mousePos) = 0;
	virtual void onMiddleMouseDown(const Point& mousePos) = 0;
	virtual void onMiddleMouseUp(const Point& mousePos) = 0;
};