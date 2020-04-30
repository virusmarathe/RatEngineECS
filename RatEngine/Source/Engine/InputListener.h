#pragma once

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
};