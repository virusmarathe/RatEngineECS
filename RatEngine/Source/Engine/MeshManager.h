#pragma once
#include "ResourceManager.h"

class MeshManager : public ResourceManager
{
public:
	MeshManager();
	~MeshManager();

	MeshPointer createMeshFromFile(const wchar_t* filePath);

protected:
	// Inherited via ResourceManager
	virtual Resource* createResourceFromFileConcrete(const wchar_t* filePath) override;
};

