#include "AppWindow.h"
#include <Windows.h>
#include "Vector3.h"
#include "Matrix4x4.h"

struct vector4
{
	float r, g, b, a;
};

struct vertex
{
	Vector3 position;
	vector4 color;
	vector4 color1;
};

_declspec(align(16))
struct constant
{
	Matrix4x4 m_World;
	Matrix4x4 m_View;
	Matrix4x4 m_Projection;
	unsigned int m_Time;
};

AppWindow::AppWindow() : Window(), m_SwapChain(NULL), m_VertexBuffer(NULL), m_VertexShader(NULL), m_PixelShader(NULL),
						 m_ConstantBuffer(NULL), m_PrevFrameTime(0), m_DeltaTime(0), m_LerpTimer(0), m_LerpDuration(10)
{
}

AppWindow::~AppWindow()
{
}

void AppWindow::updateQuadPosition()
{
	DeviceContext* context = GraphicsEngine::get()->getImmediateDeviceContext();
	constant data;
	RECT rect = getClientWindowRect();
	data.m_Time = GetTickCount();

	m_LerpTimer += m_DeltaTime;
	m_ScaleLerpTimer += m_DeltaTime;
	float lerpVal = m_LerpTimer / m_LerpDuration;
	float lerpScaleVal = m_ScaleLerpTimer / 0.5f;

	if (lerpVal > 1)
	{
		lerpVal = 1;
		m_LerpTimer = 0;
	}

	Matrix4x4 temp;
	data.m_World.setScale(Vector3::lerp(Vector3(0.5f, 0.5f, 0), Vector3(2, 2, 0), (sin(lerpScaleVal) + 1.0f) / 2.0f));
	temp.setTranslation(Vector3::lerp(Vector3(-1, -1, 0), Vector3(1, 1, 0), lerpVal));
	data.m_World *= temp;
	data.m_View.setIdentity();
	data.m_Projection.setOrthoLH((rect.right - rect.left)/400.0f, (rect.bottom - rect.top)/400.0f, -4.0f, 4.0f);


	m_ConstantBuffer->update(context, &data);
}

void AppWindow::onCreate()
{
	GraphicsEngine::get()->init();
	m_SwapChain = GraphicsEngine::get()->createSwapChain();
	RECT rc = getClientWindowRect();
	m_SwapChain->init(m_hwnd, rc.right - rc.left, rc.bottom - rc.top);

	vertex list[] =
	{
		{Vector3(-0.5f, -0.5f, 0),	1.0f,  0.0f, 0.0f, 1.0f,	0.4f,  1.0f, 0.0f, 1.0f},
		{Vector3(-0.5f,  0.5f, 0),	0.0f, 1.0f, 0.0f, 1.0f,		0.0f,  0.3f, 1.0f, 1.0f},
		{Vector3(0.5f, -0.5f, 0),	0.0f, 0.0f, 1.0f, 1.0f,		0.2f,  0.0f, 0.4f, 1.0f},
		{Vector3(0.5f,  0.5f, 0),	1.0f, 1.0f, 1.0f, 1.0f,		1.0f,  1.0f, 1.0f, 1.0f}
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

	m_PrevFrameTime = (float)GetTickCount();
}

void AppWindow::onUpdate()
{
	DWORD curTime = GetTickCount();
	m_DeltaTime = (curTime - m_PrevFrameTime) / 1000.0f;
	m_PrevFrameTime = curTime;

	DeviceContext* context = GraphicsEngine::get()->getImmediateDeviceContext();

	context->clearRenderTargetColor(m_SwapChain, 0.1f, 0.1f, 0.6f, 1);

	RECT rc = this->getClientWindowRect();
	context->setViewportSize((FLOAT)(rc.right - rc.left), (FLOAT)(rc.bottom - rc.top));

	updateQuadPosition();

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
