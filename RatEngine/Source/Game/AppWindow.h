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
#include "EulerRotatorSystem.h"
#include "RotateTimerComponent.h"
#include "FlyCamControllerSystem.h"
#include "FlyCamComponent.h"
#include "AttachToParentSystem.h"

class AppWindow : public Window
{
public:
	AppWindow();
	~AppWindow();

	// Inherited via Window
	virtual void onCreate() override;

	virtual void onUpdate() override;

	virtual void onDestroy() override;

	virtual void onFocus() override;

	virtual void onKillFocus() override;

private:
	ECS ecs;
	ECSSystemList inputSystems;
	ECSSystemList mainSystems;
	ECSSystemList renderingSystems;

	SwapChain* m_SwapChain;

	EntityHandle teapot;
	EntityHandle statue;
	EntityHandle skybox;
	EntityHandle directionalLight;
	EntityHandle camera;

	StaticMeshRenderingSystem meshRendererSystem;
	SimpleMotionSystem simpleMotionSystem;
	EulerRotatorSystem eulerRotatorSystem;
	FlyCamControllerSystem flyCamSystem;
	AttachToParentSystem attachParentSystem;
};

