#include "GraphicsEngine.h"
#include "RenderSystem.h"

#pragma warning(disable: 26812)

GraphicsEngine::GraphicsEngine() : m_RenderSystem(NULL)
{
}

GraphicsEngine::~GraphicsEngine()
{
}

bool GraphicsEngine::init()
{
	m_RenderSystem = new RenderSystem();
	m_RenderSystem->init();
	return true;
}

bool GraphicsEngine::release()
{
	m_RenderSystem->release();
	delete m_RenderSystem;
	return true;
}

GraphicsEngine* GraphicsEngine::get()
{
	static GraphicsEngine engine;
	return &engine;
}
