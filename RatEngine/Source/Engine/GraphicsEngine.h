#pragma once
#include <d3d11.h>
#include "Prerequisites.h"
#include "RenderSystem.h"

class GraphicsEngine
{
public:
	GraphicsEngine();
	~GraphicsEngine();

	// Initialize the Grpahics Engine and DirectX 11 Device
	bool init();

	// Release the resources loaded
	bool release();

	RenderSystem* getRenderSystem() { return m_RenderSystem; }

	static GraphicsEngine* get();

private:
	RenderSystem* m_RenderSystem;
};

