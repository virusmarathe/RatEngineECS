#pragma once
#include "Window.h"
#include "GraphicsEngine.h"
#include "SwapChain.h"
#include "DeviceContext.h"
#include "VertexBuffer.h"
#include "VertexShader.h"
#include "PixelShader.h"
#include "ConstantBuffer.h"
#include "IndexBuffer.h"
#include "InputListener.h"
#include "Matrix4x4.h"
#include "ECS.h"
#include "StaticMeshRenderingSystem.h"
#include "SimpleMotionSystem.h"
#include "SimpleMotionComponent.h"

class AppWindow : public Window, public InputListener
{
public:
	AppWindow();
	~AppWindow();

	void update();

	// Inherited via Window
	virtual void onCreate() override;

	virtual void onUpdate() override;

	virtual void onDestroy() override;

	virtual void onFocus() override;

	virtual void onKillFocus() override;

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

private:
	ECS ecs;
	ECSSystemList mainSystems;

	SwapChain* m_SwapChain;

	EntityHandle teapot;
	EntityHandle statue;
	EntityHandle skybox;

	StaticMeshRenderingSystem meshRendererSystem;
	SimpleMotionSystem simpleMotionSystem;

	float m_XRot = 0;
	float m_YRot = 0;

	Matrix4x4 m_CameraTransform;
	float m_ForwardDirection = 0.0f;
	float m_RightDirection = 0.0f;
	float m_LightRotY = 0.0f;
};

