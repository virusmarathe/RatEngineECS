#include "AppWindow.h"
#include <Windows.h>

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
	vector3 position1;
	vector4 color;
	vector4 color1;
};

_declspec(align(16))
struct constant
{
	unsigned int m_Time;
};

AppWindow::AppWindow() : Window(), m_SwapChain(NULL), m_VertexBuffer(NULL), m_VertexShader(NULL), m_PixelShader(NULL),
						 m_ConstantBuffer(NULL)
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
		{-0.5f, -0.5f, 0,		-0.32f, -0.11f, 0,	1.0f,  0.0f, 0.0f, 1.0f,	0.4f,  1.0f, 0.0f, 1.0f},
		{-0.5f,  0.5f, 0,		-0.11f, 0.78f, 0,	0.0f, 1.0f, 0.0f, 1.0f,		0.0f,  0.3f, 1.0f, 1.0f},
		{ 0.5f, -0.5f, 0,		0.75f, -0.75f, 0,	0.0f, 0.0f, 1.0f, 1.0f,		0.2f,  0.0f, 0.4f, 1.0f},
		{ 0.5f,  0.5f, 0,		0.88f, 0.88f, 0,	1.0f, 1.0f, 1.0f, 1.0f,		1.0f,  1.0f, 1.0f, 1.0f}
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

	constant data;
	data.m_Time = 0;

	m_ConstantBuffer = GraphicsEngine::get()->createConstantBuffer();
	m_ConstantBuffer->load(&data, sizeof(constant));
}

void AppWindow::onUpdate()
{
	DeviceContext* context = GraphicsEngine::get()->getImmediateDeviceContext();

	context->clearRenderTargetColor(m_SwapChain, 0.1f, 0.1f, 0.6f, 1);

	RECT rc = this->getClientWindowRect();
	context->setViewportSize((FLOAT)(rc.right - rc.left), (FLOAT)(rc.bottom - rc.top));

	constant data;
	data.m_Time = GetTickCount();

	m_ConstantBuffer->update(context, &data);

	context->setConstantBuffer(m_PixelShader, m_ConstantBuffer);
	context->setConstantBuffer(m_VertexShader, m_ConstantBuffer);

	context->setVertexShader(m_VertexShader);
	context->setPixelShader(m_PixelShader);
	context->setVertexBuffer(m_VertexBuffer);
	context->drawTriangleStrip(m_VertexBuffer->getNumVertices(), 0);

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
