#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"
#include "TextureManager.h"

class GraphicsEngine
{
private:
	GraphicsEngine();
	~GraphicsEngine();

public:
	RenderSystem* getRenderSystem() { return m_RenderSystem; }
	TextureManager* getTextureManager() { return m_TextureManager; }

	static GraphicsEngine* get();
	static void create();
	static void release();


private:
	RenderSystem* m_RenderSystem;
	TextureManager* m_TextureManager;
	static GraphicsEngine* m_GraphicsEngine;
};

