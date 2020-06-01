#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "FlyCamComponent.h"
#include "InputListener.h"
#include <Windows.h>

class FlyCamControllerSystem : public BaseECSSystem, public InputListener
{
public:
	FlyCamControllerSystem();

	virtual void updateComponents(float deltaTime, BaseECSComponent** components);
	virtual void cleanup() override;

	// Inherited via InputListener
	virtual void onKeyDown(int key) override;
	virtual void onKeyUp(int key) override;
	virtual void onMouseMove(const Point& mousePos) override;
	virtual void onLeftMouseDown(const Point& mousePos) override;
	virtual void onLeftMouseUp(const Point& mousePos) override;
	virtual void onRightMouseDown(const Point& mousePos) override;
	virtual void onRightMouseUp(const Point& mousePos) override;
	virtual void onMiddleMouseDown(const Point& mousePos) override;
	virtual void onMiddleMouseUp(const Point& mousePos) override;

	RECT clientWindowRect;

private:
	float m_XRot = 0;
	float m_YRot = 0;
	float m_ForwardDirection = 0.0f;
	float m_RightDirection = 0.0f;
};
