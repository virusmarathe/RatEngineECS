#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"
#include "TextureManager.h"
#include "MeshManager.h"
#include "MaterialManager.h"

class GraphicsEngine
{
private:
	GraphicsEngine();
	~GraphicsEngine();

public:
	RenderSystem* getRenderSystem() { return m_RenderSystem; }
	TextureManager* getTextureManager() { return m_TextureManager; }
	MeshManager* getMeshManager() { return m_MeshManager; }
	MaterialManager* getMaterialManager() { return m_MaterialManager; }

	static GraphicsEngine* get();
	static void create();
	static void release();


private:
	RenderSystem* m_RenderSystem;
	TextureManager* m_TextureManager;
	MeshManager* m_MeshManager;
	MaterialManager* m_MaterialManager;
	static GraphicsEngine* m_GraphicsEngine;
};

