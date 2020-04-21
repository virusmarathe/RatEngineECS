#include "AppWindow.h"

struct vector3
{
	float x, y, z;
};

struct vertex
{
	vector3 position;
};

AppWindow::AppWindow() : Window(), m_SwapChain(NULL), m_VertexBuffer(NULL)
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::onCreate()
{
	GraphicsEngine::get()->init();
	m_SwapChain = GraphicsEngine::get()->createSwapChain();
	RECT rc = getClientWindowRect();
	m_SwapChain->init(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex list[] =
	{
		{-0.5f, -0.5f, 0},
		{-0.5f,  0.5f, 0},
		{ 0.5f, -0.5f, 0},
		{ 0.5f,  0.5f, 0}
	};

	GraphicsEngine::get()->createShaders();
	void* shaderByteCode = nullptr;
	UINT shaderSize = 0;
	GraphicsEngine::get()->getShaderBufferAndSize(&shaderByteCode, &shaderSize);

	m_VertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	UINT listSize = ARRAYSIZE(list);
	m_VertexBuffer->load(list, sizeof(vertex), listSize, shaderByteCode, shaderSize);
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_VertexBuffer);
}

void AppWindow::onUpdate()
{
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(m_SwapChain, 0.1f, 0.1f, 0.6f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize((FLOAT)(rc.right - rc.left), (FLOAT)(rc.bottom - rc.top));
	GraphicsEngine::get()->setShaders();
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_VertexBuffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_VertexBuffer->getNumVertices(), 0);

	m_SwapChain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_VertexBuffer->release();
	m_SwapChain->release();
	GraphicsEngine::get()->release();
}
