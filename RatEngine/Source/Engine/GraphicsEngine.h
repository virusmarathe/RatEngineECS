#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"

class GraphicsEngine
{
private:
	GraphicsEngine();
	~GraphicsEngine();

public:
	RenderSystem* getRenderSystem() { return m_RenderSystem; }

	static GraphicsEngine* get();
	static void create();
	static void release();


private:
	RenderSystem* m_RenderSystem;
	static GraphicsEngine* m_GraphicsEngine;
};

