#include "MaterialManager.h"
#include "Material.h"

MaterialManager::MaterialManager()
{
}

MaterialManager::~MaterialManager()
{
}

MaterialPointer MaterialManager::createMaterialFromFile(const wchar_t* filePath)
{
	return std::static_pointer_cast<Material>(createResourceFromFile(filePath));
}

Resource* MaterialManager::createResourceFromFileConcrete(const wchar_t* filePath)
{
	Material* mat = nullptr;
	try
	{
		mat = new Material(filePath);
	}
	catch (...) {}

	return mat;
}
