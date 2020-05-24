#pragma once
#include "ECS.h"
#include "Mesh.h"

struct MeshRendererComponent : public ECSComponent<MeshRendererComponent>
{
	MeshPointer mesh;
};