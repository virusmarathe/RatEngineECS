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
		MaterialPointer mat = meshRendererComponent->m_Material;

		constant data;
		data.m_LightDirection = m_LightTransform->forward();
		data.m_CameraPosition = m_CameraTransform->position();

		data.m_World.setMatrix(transformComponent->transform);

		Matrix4x4 inverseCam = *m_CameraTransform;
		inverseCam.inverse();
		data.m_View = inverseCam;

		int width = (clientWindowRect.right - clientWindowRect.left);
		int height = (clientWindowRect.bottom - clientWindowRect.top);
		data.m_Projection.setPerspectiveFovLH(1.5708f, ((float)width) / ((float)height), 0.1f, 100.0f);

		meshRendererComponent->constantBuffer->update(m_Context, &data);

		GraphicsEngine::get()->getRenderSystem()->setBackCulling(meshRendererComponent->backFaceCulled);
		
		// TODO: material->setInputLayout, vertexshader, pixel shader, constantbuffer

		m_Context->setConstantBuffer(mat->m_PixelShader, meshRendererComponent->constantBuffer);
		m_Context->setConstantBuffer(mat->m_VertexShader, meshRendererComponent->constantBuffer);

		m_Context->setVertexShader(mat->m_VertexShader);
		m_Context->setPixelShader(mat->m_PixelShader);

		m_Context->setTexture(mat->m_PixelShader, meshRendererComponent->m_Texture);

		m_Context->setVertexBuffer(mesh->getVertexBuffer());
		m_Context->setInputLayout(mat->m_InputLayout);
		m_Context->setIndexBuffer(mesh->getIndexBuffer());
		m_Context->drawIndexedTriangleList(mesh->getIndexBuffer()->getNumIndices(), 0, 0);
	}

	DeviceContext* m_Context;
	Matrix4x4* m_LightTransform;
	Matrix4x4 * m_CameraTransform;
	RECT clientWindowRect;
};