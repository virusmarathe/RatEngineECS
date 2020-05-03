#include "MeshManager.h"
#include "Mesh.h"

MeshManager::MeshManager()
{
}

MeshManager::~MeshManager()
{
}

MeshPointer MeshManager::createMeshFromFile(const wchar_t* filePath)
{
	return std::static_pointer_cast<Mesh>(createResourceFromFile(filePath));
}

Resource* MeshManager::createResourceFromFileConcrete(const wchar_t* filePath)
{
	Mesh* mesh = nullptr;
	try
	{
		mesh = new Mesh(filePath);
	}
	catch (...) {}

	return mesh;
}
