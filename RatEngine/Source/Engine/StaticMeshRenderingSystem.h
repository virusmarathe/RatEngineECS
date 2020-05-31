#pragma once
#include "ECS.h"
#include "TransformComponent.h"
#include "MeshRendererComponent.h"

_declspec(align(16))
struct constant
{
	Matrix4x4 m_World;
	Matrix4x4 m_View;
	Matrix4x4 m_Projection;
	Vector4 m_LightDirection;
	Vector3 m_CameraPosition;
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

		constant data;
		data.m_LightDirection = m_LightDirection;
		data.m_CameraPosition = m_CameraTransform.position();

		data.m_World.setMatrix(transformComponent->transform);

		Matrix4x4 inverseCam = m_CameraTransform;
		inverseCam.inverse();
		data.m_View = inverseCam;

		int width = (clientWindowRect.right - clientWindowRect.left);
		int height = (clientWindowRect.bottom - clientWindowRect.top);
		data.m_Projection.setPerspectiveFovLH(1.5708f, ((float)width) / ((float)height), 0.1f, 100.0f);

		meshRendererComponent->constantBuffer->update(m_Context, &data);

		GraphicsEngine::get()->getRenderSystem()->setBackCulling(meshRendererComponent->backFaceCulled);
		
		m_Context->setConstantBuffer(meshRendererComponent->pixelShader, meshRendererComponent->constantBuffer);
		m_Context->setConstantBuffer(mesh->getVertexShader(), meshRendererComponent->constantBuffer);

		m_Context->setVertexShader(mesh->getVertexShader());
		m_Context->setPixelShader(meshRendererComponent->pixelShader);

		m_Context->setTexture(meshRendererComponent->pixelShader, meshRendererComponent->m_Texture);

		m_Context->setVertexBuffer(mesh->getVertexBuffer());
		m_Context->setIndexBuffer(mesh->getIndexBuffer());
		m_Context->drawIndexedTriangleList(mesh->getIndexBuffer()->getNumIndices(), 0, 0);
	}

	DeviceContext* m_Context;
	Vector4 m_LightDirection;
	Matrix4x4 m_CameraTransform;
	RECT clientWindowRect;
};