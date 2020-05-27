#pragma once
#include "ECS.h"
#include "Mesh.h"

struct MeshRendererComponent : public ECSComponent<MeshRendererComponent>
{
	~MeshRendererComponent()
	{
		if (pixelShader)
			delete pixelShader;
		if (constantBuffer)
			delete constantBuffer;
	}

	MeshPointer mesh;
	PixelShader* pixelShader = nullptr;
	ConstantBuffer* constantBuffer = nullptr;
	TexturePointer m_Texture;
	bool backFaceCulled = true;
};