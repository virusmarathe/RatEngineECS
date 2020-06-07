#pragma once
#include "ResourceManager.h"

class MaterialManager : public ResourceManager
{
public:
	MaterialManager();
	~MaterialManager();

	MaterialPointer createMaterialFromFile(const wchar_t* filePath);

protected:
	// Inherited via ResourceManager
	virtual Resource* createResourceFromFileConcrete(const wchar_t* filePath) override;
};

