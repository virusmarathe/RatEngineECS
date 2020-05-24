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
#include "Mesh.h"

struct TransformComponent : public ECSComponent<TransformComponent>
{
	Matrix4x4 transform;
};

struct MeshRendererComponent : public ECSComponent<MeshRendererComponent>
{
	MeshPointer mesh;
};

class StaticMeshRenderingSystem : public BaseECSSystem
{
public:
	StaticMeshRenderingSystem() : BaseECSSystem()
	{
		m_Context = GraphicsEngine::get()->getRenderSystem()->getImmediateDeviceContext();
		addComponentType(TransformComponent::ID, 0);
		addComponentType(MeshRendererComponent::ID, 0);
	}

	virtual void updateComponents(float deltaTime, BaseECSComponent** components)
	{
		TransformComponent* transformComponent = (TransformComponent*)components[0];
		MeshRendererComponent* meshRendererComponent = (MeshRendererComponent*)components[1];

		MeshPointer mesh = meshRendererComponent->mesh;


		m_Context->setConstantBuffer(m_PixelShader, m_ConstantBuffer);
		m_Context->setConstantBuffer(mesh->getVertexShader(), m_ConstantBuffer);

		m_Context->setVertexShader(mesh->getVertexShader());
		m_Context->setPixelShader(m_PixelShader);

		//m_Context->setTexture(meshRendererSystem.m_PixelShader, m_WoodTexture);

		m_Context->setVertexBuffer(mesh->getVertexBuffer());
		m_Context->setIndexBuffer(mesh->getIndexBuffer());
		m_Context->drawIndexedTriangleList(mesh->getIndexBuffer()->getNumIndices(), 0, 0);
	}

	DeviceContext* m_Context;
	PixelShader* m_PixelShader;
	ConstantBuffer* m_ConstantBuffer;
};

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

	TexturePointer m_WoodTexture;
	EntityHandle teapot;
	EntityHandle statue;
	StaticMeshRenderingSystem meshRendererSystem;

	DWORD m_PrevFrameTime;
	float m_DeltaTime;

	float m_XRot = 0;
	float m_YRot = 0;

	Matrix4x4 m_CameraTransform;
	float m_ForwardDirection = 0.0f;
	float m_RightDirection = 0.0f;
	float m_LightRotY = 0.0f;
};

