#include "FlyCamControllerSystem.h"
#include "InputSystem.h"
#include "Matrix4x4.h"

FlyCamControllerSystem::FlyCamControllerSystem() : BaseECSSystem()
{
	addComponentType(TransformComponent::ID, 0);
	addComponentType(FlyCamComponent::ID, 0);
}

void FlyCamControllerSystem::updateComponents(float deltaTime, BaseECSComponent** components)
{
	TransformComponent* transformComponent = (TransformComponent*)components[0];
	FlyCamComponent* flyCamComponent = (FlyCamComponent*)components[1];

	Matrix4x4 worldCam;
	worldCam.setIdentity();
	Matrix4x4 temp;
	temp.setIdentity();
	temp.setRotationX(m_XRot);
	worldCam *= temp;
	temp.setIdentity();
	temp.setRotationY(m_YRot);
	worldCam *= temp;
	Vector3 pos = transformComponent->transform.position() + worldCam.forward() * (m_ForwardDirection * flyCamComponent->speed * deltaTime);
	pos = pos + worldCam.right() * (m_RightDirection * flyCamComponent->speed * deltaTime);
	temp.setIdentity();
	temp.setTranslation(pos);
	worldCam *= temp;

	transformComponent->transform.setMatrix(worldCam);
}

void FlyCamControllerSystem::cleanup()
{
}

void FlyCamControllerSystem::onKeyDown(int key)
{
	float speed = 1.0f;
	if (key == 'W')
	{
		m_ForwardDirection = 1.0f;
	}
	else if (key == 'S')
	{
		m_ForwardDirection = -1.0f;
	}
	if (key == 'A')
	{
		m_RightDirection = -1.0f;
	}
	else if (key == 'D')
	{
		m_RightDirection = 1.0f;
	}
}

void FlyCamControllerSystem::onKeyUp(int key)
{
	if (m_ForwardDirection != 0 && (key == 'W' || key == 'S'))
	{
		m_ForwardDirection = 0.0f;
	}

	if (m_RightDirection != 0 && (key == 'A' || key == 'D'))
	{
		m_RightDirection = 0.0f;
	}
}

void FlyCamControllerSystem::onMouseMove(const Point& mousePos)
{
	// TODO: better way to do mouse clamping

	int xCenter = (int)((clientWindowRect.right + clientWindowRect.left) / 2.0f);
	int yCenter = (int)((clientWindowRect.bottom - clientWindowRect.top) / 2.0f);

	float sensitivity = 0.001f;
	m_XRot += (mousePos.Y - yCenter) * sensitivity;
	m_YRot += (mousePos.X - xCenter) * sensitivity;

	InputSystem::get()->setCursorPosition(Point(xCenter, yCenter));
}

void FlyCamControllerSystem::onLeftMouseDown(const Point& mousePos)
{
}

void FlyCamControllerSystem::onLeftMouseUp(const Point& mousePos)
{
}

void FlyCamControllerSystem::onRightMouseDown(const Point& mousePos)
{
}

void FlyCamControllerSystem::onRightMouseUp(const Point& mousePos)
{
}

void FlyCamControllerSystem::onMiddleMouseDown(const Point& mousePos)
{
}

void FlyCamControllerSystem::onMiddleMouseUp(const Point& mousePos)
{
}
