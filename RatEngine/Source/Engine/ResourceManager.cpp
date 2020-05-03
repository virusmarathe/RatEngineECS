#include "ResourceManager.h"
#include <filesystem>

ResourceManager::ResourceManager()
{
}

ResourceManager::~ResourceManager()
{
}

ResourcePointer ResourceManager::createResourceFromFile(const wchar_t* filePath)
{
	std::wstring fullPath = std::filesystem::absolute(filePath);

	auto it = m_ResourceMap.find(fullPath);

	if (it != m_ResourceMap.end())
		return it->second;

	Resource* rawResource = this->createResourceFromFileConcrete(fullPath.c_str());

	if (rawResource)
	{
		ResourcePointer res(rawResource);
		m_ResourceMap[fullPath] = res;
		return res;
	}

	return nullptr;
}
