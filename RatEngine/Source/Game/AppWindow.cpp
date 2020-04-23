#include "AppWindow.h"

struct vector3
{
	float x, y, z;
};

struct vector4
{
	float r, g, b, a;
};

struct vertex
{
	vector3 position;
	vector4 color;
};

AppWindow::AppWindow() : Window(), m_SwapChain(NULL), m_VertexBuffer(NULL), m_VertexShader(NULL), m_PixelShader(NULL)
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
		{-0.5f, -0.5f, 0, 1.0f, 0.0f, 0.0f, 1.0f},
		{-0.5f,  0.5f, 0, 0.0f, 1.0f, 0.0f, 1.0f},
		{ 0.5f, -0.5f, 0, 0.0f, 0.0f, 1.0f, 1.0f},
		{ 0.5f,  0.5f, 0, 1.0f, 1.0f, 1.0f, 1.0f}
	};
	m_VertexBuffer = GraphicsEngine::get()->createVertexBuffer();
	UINT listSize = ARRAYSIZE(list);

	void* shaderByteCode = nullptr;
	SIZE_T shaderSize = 0;

	GraphicsEngine::get()->compileVertexShader(L"Source/Shaders/VertexShader.hlsl", "vsmain", &shaderByteCode, &shaderSize);
	m_VertexShader = GraphicsEngine::get()->createVertexShader(shaderByteCode, shaderSize);
	m_VertexBuffer->load(list, sizeof(vertex), listSize, shaderByteCode, shaderSize);

	GraphicsEngine::get()->compilePixelShader(L"Source/Shaders/PixelShader.hlsl", "psmain", &shaderByteCode, &shaderSize);
	m_PixelShader = GraphicsEngine::get()->createPixelShader(shaderByteCode, shaderSize);

	GraphicsEngine::get()->releaseCompiledShader();
}

void AppWindow::onUpdate()
{
	GraphicsEngine::get()->getImmediateDeviceContext()->clearRenderTargetColor(m_SwapChain, 0.1f, 0.1f, 0.6f, 1);

	RECT rc = this->getClientWindowRect();
	GraphicsEngine::get()->getImmediateDeviceContext()->setViewportSize((FLOAT)(rc.right - rc.left), (FLOAT)(rc.bottom - rc.top));
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexShader(m_VertexShader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setPixelShader(m_PixelShader);
	GraphicsEngine::get()->getImmediateDeviceContext()->setVertexBuffer(m_VertexBuffer);
	GraphicsEngine::get()->getImmediateDeviceContext()->drawTriangleStrip(m_VertexBuffer->getNumVertices(), 0);

	m_SwapChain->present(false);
}

void AppWindow::onDestroy()
{
	Window::onDestroy();
	m_VertexBuffer->release();
	m_VertexShader->release();
	m_PixelShader->release();
	m_SwapChain->release();
	GraphicsEngine::get()->release();
}
