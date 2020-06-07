#pragma once
#include "ECS.h"
#include "Mesh.h"
#include "Material.h"

struct MeshRendererComponent : public ECSComponent<MeshRendererComponent>
{
	~MeshRendererComponent()
	{
		if (constantBuffer)
			delete constantBuffer;
	}

	MeshPointer mesh;
	ConstantBuffer* constantBuffer = nullptr;
	TexturePointer m_Texture;
	MaterialPointer m_Material;
	bool backFaceCulled = true;
};